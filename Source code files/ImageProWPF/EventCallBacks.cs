using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
//using System.Windows.Forms;
using Fluent;
using System.Drawing;
using WrapCPathfind;
using System.Collections.ObjectModel;
using Microsoft.Office.Interop.Excel;



namespace ImageProWPF
{
    public partial class MainWindow : RibbonWindow
    {
        
        private void btnAppExit_Click(object sender, RoutedEventArgs e)
        {
            //System.Windows.MessageBox.Show("HELLo");
            //this.Close();
            Environment.Exit(0);
            //Application.Exit();
        }

        private void btnImgImport_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog ofd = new Microsoft.Win32.OpenFileDialog();
            ofd.Title = "Import";
            ofd.Filter = "All files(*.*)|*.*|JPG files(*.jpg)|*.jpg|BMP files(*.bmp)|*.bmp|TIFF files(*.tiff)|*.tiff|PNG files(*.png)|*.png";            
            if (ofd.ShowDialog() == true)
            {
                Bitmap tmp = new Bitmap(ofd.FileName); 
                importbitmap = tmp.Clone(new System.Drawing.Rectangle(0,0,tmp.Width,tmp.Height),System.Drawing.Imaging.PixelFormat.Format24bppRgb) as Bitmap;
                imageshowbitmapimage=BitmapToBitmapImage(importbitmap);
                imageshowbox.Source = imageshowbitmapimage;

                crackfinder.Init(importbitmap);
                dataset.Clear();
                locList.Clear();

                imageWidth = importbitmap.Width;
                imageHeight = importbitmap.Height;

                imageshowbox.Width = importbitmap.Width;
                tempbindgroup.Zoomsize = 100;
                tempbindgroup.Timetaken = 0;
                tempbindgroup.sImagesize = imageWidth.ToString() + "*" + imageHeight.ToString();
                tempbindgroup.Prompt = "Ready";
                ispicking = false;
                ismeasurepicking = false;
                btnDeleteCrack.IsEnabled = true;
                imageshowbox.Cursor = System.Windows.Input.Cursors.Hand;
                
                cracksuminfo.Arearatio = 0;
                cracksuminfo.Cracknum = 0;
  

            }
           

        }
        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (dataset.Count==0)
            {
                return;
            }
            int index=datalistview.SelectedIndex;
            crackfinder.LightSelected(index);
            imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
            imageshowbox.Source = imageshowbitmapimage;

        }
        private void imageshowbox_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            
            if (importbitmap != null)
            {
                isdragingimage = true;
                isdragingpick=false;
                clickdownPositionInimage=e.GetPosition((IInputElement)imageshowbox);
            }
            
        }
        private void imageshowbox_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (isdragingimage)
            {
                isdragingpick=true;
                System.Windows.Point ptINviewer = e.GetPosition((IInputElement)imagescrollviewer);
                double newoffsetx = clickdownPositionInimage.X - ptINviewer.X;
                double newoffsety = clickdownPositionInimage.Y - ptINviewer.Y;
                if (newoffsetx > 0)
                {
                    imagescrollviewer.ScrollToHorizontalOffset(newoffsetx);
                }
                if (newoffsety > 0)
                {
                    imagescrollviewer.ScrollToVerticalOffset(newoffsety);
                }
                
            }

        }

        private void imageshowbox_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            isdragingimage = false;
            if (importbitmap != null && ispicking && !isdragingpick)
            {
                System.Windows.Point pt = e.GetPosition((IInputElement)imageshowbox);
                CLRLocation x = new CLRLocation();

                x.Row = (int)(pt.Y * imageWidth / imageshowbox.ActualWidth);
                x.Col = (int)(pt.X * imageWidth / imageshowbox.ActualWidth);

                if (locList.Count == 0)
                {
                    crackfinder.MarkPoint(x, true);
                }
                else
                {
                    crackfinder.MarkPoint(x, false);
                }
                locList.Add(x);



                /*if (clicknum==0)
                {
                    startlocation = x;
                    crackfinder.MarkPoint(x,true);
                }
                else if (clicknum == 1)
                {
                    goallocation = x;
                    crackfinder.MarkPoint(x, false);
                }   */
                imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapformarkshow);
                imageshowbox.Source = imageshowbitmapimage;

                if (locList.Count == 2 && !ismeasurepicking)
                {
                    btnCalculate.IsEnabled = true;
                }
                else if (locList.Count == 2 && ismeasurepicking)
                {
                    CLRLocation x1 = locList[0];
                    CLRLocation x2 = locList[1];
                    tempbindgroup.Measuredistance = System.Math.Sqrt((x1.Row - x2.Row) * (x1.Row - x2.Row) + (x1.Col - x2.Col) * (x1.Col - x2.Col));
                    locList.Clear();
                    btnPickCrackPoint.IsEnabled = true;
                    tempbindgroup.Prompt = "Ready";
                    imageshowbox.Cursor = System.Windows.Input.Cursors.Hand;
                    ismeasurepicking = false;
                    ispicking = false;
                    imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
                    imageshowbox.Source = imageshowbitmapimage;
                }
            }

        }
        private void imageshowbox_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            isdragingimage = false;
        }
        private void imagescrollviewer_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (ispicking)
            {
                //clicknum = 0;
                locList.Clear();
                btnCalculate.IsEnabled = false;
                btnPickCrackPoint.IsEnabled = true;
                btnMeasurePick.IsEnabled = true;
                ispicking = false;
                btnDeleteCrack.IsEnabled = true;
                imageshowbox.Cursor = System.Windows.Input.Cursors.Hand;
                tempbindgroup.Prompt = "Ready";
                imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
                imageshowbox.Source = imageshowbitmapimage;
                
            }
            
        }


        
        private void imagescrollviewer_MouseWheel(object sender, MouseWheelEventArgs e)
        {

            
            
            if (importbitmap==null || isdragingimage)
            {
                e.Handled = true;
                return;
            }
            if (e.Delta>0)
            {
                tempbindgroup.Zoomsize += zoomdelta;
            }
            else
            {
                tempbindgroup.Zoomsize -= zoomdelta;

            }
            
            //double x = imagescrollviewer.ContentHorizontalOffset;
            //double y = imagescrollviewer.ContentVerticalOffset;
            System.Windows.Point ptINimage = e.GetPosition((IInputElement)imageshowbox);
            System.Windows.Point ptINviewer = e.GetPosition((IInputElement)imagescrollviewer);
            double newXinimage = ptINimage.X * imageWidth * tempbindgroup.Zoomsize / 100 / imageshowbox.Width;
            double newYinimage = ptINimage.Y * imageWidth * tempbindgroup.Zoomsize / 100 / imageshowbox.Width;
            double newoffsetx = newXinimage - ptINviewer.X;
            double newoffsety = newYinimage - ptINviewer.Y;
            imageshowbox.Width = (double)imageWidth * tempbindgroup.Zoomsize / 100;
            
            if (newoffsetx>0)
            {
                imagescrollviewer.ScrollToHorizontalOffset(newoffsetx);
            }
            if (newoffsety > 0)
            {
                imagescrollviewer.ScrollToVerticalOffset(newoffsety);
            }
            e.Handled = true;
            
        }

        private void btnPickCrackPoint_Click(object sender, RoutedEventArgs e)
        {
            if (importbitmap != null)
            {
                imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
                imageshowbox.Source = imageshowbitmapimage;
                locList.Clear();
                btnMeasurePick.IsEnabled = false;
                btnCalculate.IsEnabled = false;
                ispicking = true;
                btnDeleteCrack.IsEnabled = false;
                imageshowbox.Cursor = System.Windows.Input.Cursors.Cross;
                tempbindgroup.Prompt = "!Picking Mode";
                
            }
            

        }

        private void btnmeasure_Click(object sender, RoutedEventArgs e)
        {
            if (importbitmap != null)
            {
                imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
                imageshowbox.Source = imageshowbitmapimage;
                locList.Clear();
                btnPickCrackPoint.IsEnabled = false;
                btnCalculate.IsEnabled = false;
                ismeasurepicking = true;
                ispicking = true;
                imageshowbox.Cursor = System.Windows.Input.Cursors.Cross;
                tempbindgroup.Prompt = "!Picking Mode";

            }
        }

        private void btncalibrate_Click(object sender, RoutedEventArgs e)
        {
            if (tempbindgroup.Measuredistance>0)
            {
                tempbindgroup.Pixelsize = tempbindgroup.Actualdistance / tempbindgroup.Measuredistance;
                foreach (CLRCrackParas v in dataset)
                {
                    v.UnitTransform(tempbindgroup.Pixelsize / previouspixelsize);

                }

                previouspixelsize = tempbindgroup.Pixelsize;
            }
            
        }
        private void actualdistancetextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            //此处是将输入的数字copy到变量中
            actualdistancetextBox.GetBindingExpression(System.Windows.Controls.TextBox.TextProperty).UpdateSource();
            tempbindgroup.Actualdistance = tempbindgroup.Actualdistance;//为了防止输入非法字符，添加了这句保持文本框的内容一致
        }

        private void btnzoomin_Click(object sender, RoutedEventArgs e)
        {
            tempbindgroup.Zoomsize += 10;
            imageshowbox.Width = (double)imageWidth * tempbindgroup.Zoomsize / 100;
        }

        private void btnzoomout_Click(object sender, RoutedEventArgs e)
        {
            tempbindgroup.Zoomsize -= 10;
            imageshowbox.Width = (double)imageWidth * tempbindgroup.Zoomsize / 100;

        }

        private void btnCalculate_Click(object sender, RoutedEventArgs e)
        {
            if (locList.Count<2)
            {
                return;
            }

            //这里使用了新的线程，否则UI线程阻塞，不会更新文本框信息，使用按键调用不会更改鼠标形状
            //更新提示信息，将鼠标更改为等待状态
            //new System.Threading.Thread(InvokeUpdateUIstatus).Start();           
            tempbindgroup.Prompt = "Running.....";
            this.Cursor = System.Windows.Input.Cursors.Wait;
            ispicking = false;
            btnDeleteCrack.IsEnabled = true;
            imageshowbox.Cursor = System.Windows.Input.Cursors.Hand;
            
            
            

            //process(progressbar1.Maximum * 0.3);
            tempbindgroup.Timetaken = 0;
            timer.Start();
            bool b=crackfinder.Calculate(locList.ToArray(), advancesetting);
            if (!b)
            {
                //process(progressbar1.Maximum);
                //timer.Stop();
                //process(-1);
                locList.Clear();
                btnCalculate.IsEnabled = false;
                this.Cursor = System.Windows.Input.Cursors.Arrow;
                if(MessageBox.Show("Sorry, the process is timeout, please choose a smaller tolerate value and try again!","Timeout",
                    MessageBoxButton.OK, MessageBoxImage.Information) == MessageBoxResult.OK)
                {
                    imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
                    imageshowbox.Source = imageshowbitmapimage;
                }                
                return;
            }
    
            imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
            imageshowbox.Source = imageshowbitmapimage;
            //process(progressbar1.Maximum);
            timer.Stop();
            //process(-1);

            CLRCrackParas tmp = crackfinder.crackparas.Clone();
            tmp.UnitTransform(tempbindgroup.Pixelsize);
            dataset.Add(tmp);
            cracksuminfo.Cracknum++;
            cracksuminfo.Arearatio += tmp.Area / (imageWidth * imageHeight * tempbindgroup.Pixelsize * tempbindgroup.Pixelsize);

            locList.Clear();
            btnCalculate.IsEnabled = false;
            btnMeasurePick.IsEnabled = true;

            tempbindgroup.Prompt = "Ready";
            this.Cursor = System.Windows.Input.Cursors.Arrow;

        }

        private void btnDeleteCrack_Click(object sender, RoutedEventArgs e)
        {
            if (!ispicking && importbitmap!=null)
            {
                int index = datalistview.SelectedIndex;
                if (index<0 || index >=dataset.Count)
                {
                    return;
                }
                crackfinder.DeleteCrack(index);
                imageshowbitmapimage = BitmapToBitmapImage(crackfinder.bitmapforshow);
                imageshowbox.Source = imageshowbitmapimage;
                cracksuminfo.Cracknum--;
                cracksuminfo.Arearatio -= dataset[index].Area / (imageWidth * imageHeight * tempbindgroup.Pixelsize * tempbindgroup.Pixelsize);
                dataset.RemoveAt(index);
                for (int i = index; i < dataset.Count;i++ )
                {
                    CLRCrackParas v = dataset[i];
                    v.No--;
                    
                }
            }

        }
        /*
        private void pixelsizetextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            pixelsizetextBox.GetBindingExpression(System.Windows.Controls.TextBox.TextProperty).UpdateSource();
            tempbindgroup.Pixelsize = tempbindgroup.Pixelsize;//保持文本框的内容一致

            foreach (CLRCrackParas v in dataset)
            {
                v.UnitTransform(tempbindgroup.Pixelsize/previouspixelsize);

            }

            previouspixelsize = tempbindgroup.Pixelsize;
            /*
            ObservableCollection<CLRCrackParas> temp = new ObservableCollection<CLRCrackParas>();
            foreach (CLRCrackParas v in dataset)
            {
                CLRCrackParas x = v;
                v.Length *= statuscontext.Pixelsize;
                x.Maxwidth *= statuscontext.Pixelsize;
                x.Minwidth *= statuscontext.Pixelsize;
                x.Meanwidth *= statuscontext.Pixelsize;
                x.Area *= square;
                temp.Add(x);
                 
            }
             
            //dataset = temp;
            ObservableCollection<CLRCrackParas> temp = dataset;
            if (temp.Count!=0)
            {
                CLRCrackParas x = temp[0];
                x.No = 100;
            }

            dataset = temp;
        }*/

        private void btnSaveCrackdata_Click(object sender, RoutedEventArgs e)
        {
            if (importbitmap == null)
            {
                return;
            }
            if (dataset.Count==0)
            {
                return;
            }
            Microsoft.Win32.SaveFileDialog sfd = new Microsoft.Win32.SaveFileDialog();
            sfd.DefaultExt = "xls";
            sfd.AddExtension = false;
            sfd.OverwritePrompt = true;
            sfd.FileName = "CrackData.xls";
            sfd.CheckPathExists = true;
            sfd.RestoreDirectory = true;
            sfd.Filter = "Excel2003|*.xls";
            sfd.Title = "Save to EXCEL";
            String pathstr = "";
            if (sfd.ShowDialog() == true)
            {
                pathstr = sfd.FileName;
            }
            else
            {
                return;
            }

            Microsoft.Office.Interop.Excel.Application excelApp = new Microsoft.Office.Interop.Excel.Application();
            Workbook excelWB = excelApp.Workbooks.Add(System.Type.Missing);  
            Worksheet excelWS = (Worksheet)excelWB.Worksheets[1];

            excelWS.Cells[1, 1] = "Actual width of each pixel：";
            excelWS.Cells[1, 2] = tempbindgroup.Pixelsize.ToString();
            excelWS.Cells[1, 3] = "Crack amount：";
            excelWS.Cells[1, 4] = cracksuminfo.Cracknum.ToString();
            excelWS.Cells[1, 5] = "Crack area ratio：";
            excelWS.Cells[1, 6] = cracksuminfo.Arearatio.ToString();

            excelWS.Cells[2, 1] = "No.";
            excelWS.Cells[2, 2] = "Length";
            excelWS.Cells[2, 3] = "Maximum width";
            excelWS.Cells[2, 4] = "Minimum width";
            excelWS.Cells[2, 5] = "Mean width";
            excelWS.Cells[2, 6] = "Angle";
            excelWS.Cells[2, 7] = "Area";
            for (int i = 0; i < dataset.Count;i++ )
            {
                excelWS.Cells[i+3,1]=dataset[i].No.ToString();
                excelWS.Cells[i+3,2]=dataset[i].Length.ToString();
                excelWS.Cells[i+3,3]=dataset[i].Maxwidth.ToString();
                excelWS.Cells[i+3,4]=dataset[i].Minwidth.ToString();
                excelWS.Cells[i+3,5]=dataset[i].Meanwidth.ToString();
                excelWS.Cells[i+3,6]=dataset[i].Angle.ToString();
                excelWS.Cells[i+3,7]=dataset[i].Area.ToString();
            }

            excelWB.SaveAs(pathstr); 
            excelWB.Close();
            excelApp.Quit(); 
        }
        private void btnCrackImgSave_Click(object sender, RoutedEventArgs e)
        {
            if (importbitmap==null)
            {
                return;
            }
            Microsoft.Win32.SaveFileDialog sfd = new Microsoft.Win32.SaveFileDialog();
            sfd.DefaultExt = "jpg";
            sfd.AddExtension = false;
            sfd.OverwritePrompt = true;
            sfd.FileName = "CrackImage";
            sfd.CheckPathExists = true;
            sfd.RestoreDirectory = true;
            sfd.Filter="JPG files(*.jpg)|*.jpg|BMP files(*.bmp)|*.bmp|All files(*.*)|*.*";
            sfd.Title = "Save Image";
            if (sfd.ShowDialog() == true)
            {
                crackfinder.bitmapforshow.Save(sfd.FileName);
            }
        }

        private void btnAboutAuthor_Click(object sender, RoutedEventArgs e)
        {
            AboutAuthorBox x = new AboutAuthorBox();
            
            x.WindowStartupLocation = WindowStartupLocation.CenterScreen;
            x.Show();
            
        }
        private void btnPrograminfo_Click(object sender, RoutedEventArgs e)
        {
            PrograminfoBox x = new PrograminfoBox();
            x.WindowStartupLocation = WindowStartupLocation.CenterScreen;
            x.Show();
        }

/*

        private void closeradiusTextboxLostFocus(object sender, RoutedEventArgs e)
        {
            advancesetting.Closeradius = advancesetting.Closeradius;
        }*/
        private void originthreshTextboxLostFocus(object sender, RoutedEventArgs e)
        {
            advancesetting.Tolerate = advancesetting.Tolerate;
        }
     /*   private void btnadvanceddefault_Click(object sender, RoutedEventArgs e)
        {
            advancesetting.Iscrackdark = true;
            advancesetting.Isclose = true;
            advancesetting.Closeradius = 20;
            advancesetting.Isbinaryreconstruct = false;
            advancesetting.Binarymethod = 0;
            advancesetting.Tolerate= 20;
            advancesetting.Isinterpolate = false;
            advancesetting.Interpolatemethod = 0;
        }
*/
        private void windowsKeydown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == Key.Enter && btnCalculate.IsEnabled == true)
            {
                this.Cursor = System.Windows.Input.Cursors.Wait;
                btnCalculate_Click(null, null);
            }
            if (e.Key == Key.S && btnPickCrackPoint.IsEnabled == true && !isdragingimage)
            {
                btnPickCrackPoint_Click(null, null);
            }

        }



        private void btntest_Click(object sender, RoutedEventArgs e)
        {
            if (importbitmap == null)
            {
                return;
            }
            //CLRLocation x = new CLRLocation();
           /* for (int i = 0; i < 50;i++ )
            {
                x.Row = 100 + i * 60;
                x.Col = 1300;
                locList.Add(x);
                x.Col =1800;
                locList.Add(x);               
                btnCalculate_Click(null,null);
            }*/
           /* x.Row = 16;
            x.Col = 209;
            locList.Add(x);
            x.Row = 508;
            x.Col = 562;
            locList.Add(x);
            btnCalculate_Click(null, null);*/
        }

    }
}