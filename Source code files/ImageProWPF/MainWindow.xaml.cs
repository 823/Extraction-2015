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
using System.Windows.Forms;
using Fluent;
using System.Drawing;
using WrapCPathfind;
using System.IO;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Globalization;

namespace ImageProWPF
{
	/// <summary>
	/// MainWindow.xaml 的交互逻辑
	/// </summary>
	public partial class MainWindow : RibbonWindow
	{
                
		public MainWindow()
		{
			this.InitializeComponent();   
            datalistview.ItemsSource=dataset;
            statusbar.DataContext = tempbindgroup;
            calibrationgroupbox.DataContext = tempbindgroup;
            suminfolistbox.DataContext = cracksuminfo;
            advancedExpander.DataContext = advancesetting;

            timer.Elapsed += new System.Timers.ElapsedEventHandler(timer_Tick);
            timer.Interval = 10;
            timer.AutoReset = true;
            
		}

        private Bitmap importbitmap = null;
        private int imageWidth = 0;
        private int imageHeight = 0;

        private BitmapImage imageshowbitmapimage = null;
        private ObservableCollection<CLRCrackParas> dataset = new ObservableCollection<CLRCrackParas>();
        private CLRcracksuminfo cracksuminfo = new CLRcracksuminfo();
        private TempbindingGroup tempbindgroup = new TempbindingGroup();
        private CLROptionalSET advancesetting = new CLROptionalSET();

        private CWrapPathfinding crackfinder = new CWrapPathfinding();
        private List<CLRLocation> locList = new List<CLRLocation>();
        //private CLRLocation startlocation = new CLRLocation();
        //private CLRLocation goallocation = new CLRLocation();
        //标记变量
        private bool ismeasurepicking = false;
        private bool ispicking = false;
        //private int clicknum = 0;
        private bool isdragingimage = false;
        private bool isdragingpick = false;


        private const int zoomdelta = 10;
        private double previouspixelsize = 1;
        private System.Windows.Point clickdownPositionInimage = new System.Windows.Point();

        /// <summary>
        /// binding var
        /// </summary>
        public class TempbindingGroup : AutoUpdateBase
        {
            private double pixelsize = 1;
            public double Pixelsize
            {
                get { return pixelsize; }
                set
                {
                    if (value > 0)
                    {
                        pixelsize = value;
                        OnPropertyChanged("Pixelsize");
                    }
                }
            }
            private double measuredistance = 1;
            public double Measuredistance
            {
                get { return measuredistance; }
                set
                {
                    if (value > 0)
                    {
                        measuredistance = value;
                        OnPropertyChanged("Measuredistance");
                    }
                }
            }
            private double actualdistance = 1;
            public double Actualdistance
            {
                get { return actualdistance; }
                set
                {
                    if (value > 0)
                    {
                        actualdistance = value;
                        OnPropertyChanged("Actualdistance");
                    }
                }
            }




            private int zoomsize = 100;
            public int Zoomsize
            {
                get { return zoomsize; }
                set {
                    if (value < 10)
                    {
                        zoomsize = 10;
                    }
                    else if (value > 500)
                    {
                        zoomsize = 500;
                    }
                    else
                    {
                        zoomsize = value;
                    }

                    OnPropertyChanged("Zoomsize");
                }
            }
            private int timetaken = 0;
            public int Timetaken
            {
                get { return timetaken; }
                set { if (value >= 0) { timetaken = value; OnPropertyChanged("Timetaken"); } }
            }

            private String simagesize = "";
            public String sImagesize
            {
                get { return simagesize; }
                set { simagesize = value; OnPropertyChanged("sImagesize"); }
            }
            
            private String prompt = "Ready";
            public String Prompt
            {
                get { return prompt; }
                set { prompt = value; OnPropertyChanged("Prompt"); }
            }

        }
        public class CLRcracksuminfo:AutoUpdateBase
        {
            private int cracknum=0;
            public int Cracknum
            {
                get { return cracknum; }
                set { cracknum = value; OnPropertyChanged("Cracknum"); }
            }
            private double arearatio=0;
            public double Arearatio
            {
                get { return arearatio; }
                set { arearatio = value; OnPropertyChanged("Arearatio"); }
            }
        }
       /* public class AdvancedSet : AutoUpdateBase
        {
            private bool iscrackdark = true;
            public bool Iscrackdark
            {
                get { return iscrackdark; }
                set { iscrackdark = value; OnPropertyChanged("Iscrackdark"); }
            }
            /// <summary>
            /// 
            /// </summary>
            private bool isclose = true;
            public bool Isclose
            {
                get { return isclose; }
                set { isclose = value; OnPropertyChanged("Isclose"); }
            }
            private int closeradius = 20;
            public int Closeradius
            {
                get { return closeradius; }
                set
                {
                    if (value > 0)
                    {

                        closeradius = (int)value;
                        OnPropertyChanged("Closeradius");
                    }

                    
                }
            }
            /// <summary>
            /// 
            /// </summary>
            private bool ismultipick =false;
            public bool IsMultipick
            {
                get { return ismultipick; }
                set { ismultipick = value; OnPropertyChanged("IsMultipick"); }
            }
            /// <summary>
            /// 
            /// </summary>
            private bool isoriginreconstruct = false;
            public bool Isoriginreconstruct
            {
                get { return isoriginreconstruct; }
                set { isoriginreconstruct = value; OnPropertyChanged("Isoriginreconstruct"); }
            }
            private int tolerate = 50;
            public int Tolerate
            {
                get { return tolerate; }
                set {
                    if (value>0 )
                    {
                        tolerate = value; OnPropertyChanged("Tolerate");
                    }
                     }
            }
            /// <summary>
            /// ////////
            /// </summary>
            private bool isinterpolate = true;
            public bool Isinterpolate
            {
                get { return isinterpolate; }
                set { isinterpolate = value; OnPropertyChanged("Isinterpolate"); }
            }
            private int interpolatemethod = 0;
            public int Interpolatemethod
            {
                get { return interpolatemethod; }
                set { interpolatemethod = value; OnPropertyChanged("Interpolatemethod"); }
            }
        }*/


   
        public BitmapImage BitmapToBitmapImage(Bitmap bitmap)
        {
            MemoryStream ms = new MemoryStream();
            bitmap.Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);

            BitmapImage bitmapImage = new BitmapImage();
            bitmapImage.BeginInit();
            bitmapImage.StreamSource = new MemoryStream(ms.ToArray());
            bitmapImage.EndInit();

            return bitmapImage;
        }
        System.Timers.Timer timer = new System.Timers.Timer();
        private delegate void UpdateProgressBarDelegate(System.Windows.DependencyProperty dp, Object value);
        void timer_Tick(object sender, System.Timers.ElapsedEventArgs e)
        {
            tempbindgroup.Timetaken += (int)timer.Interval;
            /*
            double value=progressbar1.Value;
            if (statuscontext.Timetaken==0)
            {
                value = 0;
            }
            value += 10;
            if (value>progressbar1.Maximum)
            {
                value -= progressbar1.Maximum;
            }
            statuscontext.Timetaken += 10;
            UpdateProgressBarDelegate updatePbDelegate = new UpdateProgressBarDelegate(progressbar1.SetValue);
            Dispatcher.Invoke(updatePbDelegate,
                    System.Windows.Threading.DispatcherPriority.Background,
                    new object[] { System.Windows.Controls.ProgressBar.ValueProperty, progressbar1.Value+100 });*/
        }



    




/*
        void process(double it)
        {
            double value = progressbar1.Value;
            UpdateProgressBarDelegate updatePbDelegate = new UpdateProgressBarDelegate(progressbar1.SetValue);
            if (it<0)
            {
                Dispatcher.Invoke(updatePbDelegate,
                        System.Windows.Threading.DispatcherPriority.Background,
                        new object[] { System.Windows.Controls.ProgressBar.ValueProperty, progressbar1.Minimum });

            }
            else
            {
                do
                {
                    value += 10;
                    Dispatcher.Invoke(updatePbDelegate,
                            System.Windows.Threading.DispatcherPriority.Background,
                            new object[] { System.Windows.Controls.ProgressBar.ValueProperty, value });
                } while (value <= it);

            }
            
        }*/



	}

    public class AutoUpdateBase : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }

    public class PixelSizeRule : ValidationRule
    {
        private int minvalue = 0;
        public int Minvalue
        {
            get { return minvalue; }
            set { minvalue = value; }
        }
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            double pixelsize = 0;

            try
            {
                if (((string)value).Length > 0)
                    pixelsize = Double.Parse((String)value);
            }
            catch (Exception e)
            {
                return new ValidationResult(false, "非法字符-只能输入数字");
            }

            if (pixelsize <= Minvalue)
            {
                return new ValidationResult(false,"不能小于0");
            }
            else
            {
                return new ValidationResult(true, null);
            }
        }
    }
    
}