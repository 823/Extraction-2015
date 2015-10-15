// WrapCPathfind.h

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace Emgu::CV;
using namespace Emgu::CV::Structure;
using namespace Emgu::CV::CvEnum;

namespace WrapCPathfind {

	public value struct CLRLocation
	{
	private:
		int row;
		int col;
	public:
		property int Row
		{
			int get(){ return row;}
			void set(int value){ row=value;}
		}
		property int Col
		{
			int get(){ return col;}
			void set(int value){ col=value;}
		}

	};

	public ref class CLRCrackParas:INotifyPropertyChanged
	{
	private:
		int no;
		double length;
		double maxwidth;
		double minwidth;
		double meanwidth;
		double angle;
		double area;
		
	public:
		property int No
		{
			int get(){ return no;}
			void set(int value){ no=value; OnPropertyChanged("No");}
		}
		property double Length
		{
			double get(){ return length;}
			void set(double value){ length=value;OnPropertyChanged("Length");}
		}
		property double Maxwidth
		{
			double get(){ return maxwidth;}
			void set(double value){ maxwidth=value;OnPropertyChanged("Maxwidth");}
		}
		property double Minwidth
		{
			double get(){ return minwidth;}
			void set(double value){ minwidth=value;OnPropertyChanged("Minwidth");}
		}
		property double Meanwidth
		{
			double get(){ return meanwidth;}
			void set(double value){ meanwidth=value;OnPropertyChanged("Meanwidth");}
		}
		property double Angle
		{
			double get(){ return angle;}
			void set(double value){ angle=value;OnPropertyChanged("Angle");}
		}
		property double Area
		{
			double get(){ return area;}
			void set(double value){ area=value;OnPropertyChanged("Area");}
		}

		CLRCrackParas^ Clone()
		{
			CLRCrackParas^ x=gcnew CLRCrackParas();
			x->No=No;
			x->Length=Length;
			x->Area=Area;
			x->Maxwidth=Maxwidth;
			x->Minwidth=Minwidth;
			x->Meanwidth=Meanwidth;
			x->Angle=Angle;
			return x;

		}
		
		//copy from unmanaged type
		void Copyfrom(CrackParas& x)
		{
			No=x.no;
			Length=x.length;
			Area=x.area;
			Maxwidth=x.maxwidth;
			Minwidth=x.minwidth;
			Meanwidth=x.meanwidth;
			Angle=x.angle;

		}

		void UnitTransform(double pixelsize)
		{
			Length*=pixelsize;
			Area*=(pixelsize*pixelsize);
			Maxwidth*=pixelsize;
			Minwidth*=pixelsize;
			Meanwidth*=pixelsize;

		}

		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ propertyName)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(propertyName));
		}
		
	};

	public ref class CLROptionalSET:INotifyPropertyChanged
	{
	private:
		bool isinvert;

		bool iscustomt;

		bool ispresmooth;
		//int closeradius;
		//bool ismultipick;

		//bool isbinaryreconstruct;
		//int binarymethod;
		//bool isoriginreconstruct;
		int tolerate;

		//bool isinterpolate;
		//int interpolatemethod;


	public:
		property bool Isinvert
		{
			bool get(){ return isinvert;}
			void set(bool value){ isinvert=value; OnPropertyChanged("Isinvert");}
		}
		property bool Iscustomt
		{
			bool get(){ return iscustomt;}
			void set(bool value){ iscustomt=value;OnPropertyChanged("Iscustomt");}
		}
		property bool Ispresmooth
		{
			bool get(){ return ispresmooth;}
			void set(bool value){ ispresmooth=value;OnPropertyChanged("Ispresmooth");}
		}
		/*property int Closeradius
		{
			int get(){ return closeradius;}
			void set(int value){ closeradius=value;OnPropertyChanged("Closeradius");}
		}
		property bool Isbinaryreconstruct
		{
			bool get(){ return isbinaryreconstruct;}
			void set(bool value){ isbinaryreconstruct=value;OnPropertyChanged("Isbinaryreconstruct");}
		}
		property int Binarymethod
		{
			int get(){ return binarymethod;}
			void set(int value){ binarymethod=value;OnPropertyChanged("Binarymethod");}
		}
		property bool Isoriginreconstruct
		{
			bool get(){ return isoriginreconstruct;}
			void set(bool value){ isoriginreconstruct=value;OnPropertyChanged("Isoriginreconstruct");}
		}*/
		property int Tolerate
		{
			int get(){ return tolerate;}
			void set(int value){ 
				if (value>0 && value<=255)
				{
					tolerate=value;
					OnPropertyChanged("Tolerate");
				}  
				
			}
		}
		/*property bool Isinterpolate
		{
			bool get(){ return isinterpolate;}
			void set(bool value){ isinterpolate=value;OnPropertyChanged("Isinterpolate");}
		}
		property int Interpolatemethod
		{
			int get(){ return interpolatemethod;}
			void set(int value){ interpolatemethod=value;OnPropertyChanged("Interpolatemethod");}
		}*/

		CLROptionalSET()
		{
			Isinvert=false;//表示图像是否反转，是表示反转，默认不反转
			Iscustomt=false;
			Ispresmooth=false;
			//Closeradius=20;
			//attention:mutual exclusion
			//Isbinaryreconstruct=false;
			//Isoriginreconstruct=true;
			//
			//Binarymethod=0;
			Tolerate=20;
			//Isinterpolate=false;
			//Interpolatemethod=0;

		}

		//copy to unmanaged type
		void Copyto(OptionalSET& x)
		{
			x.isinvert=Isinvert;
			x.iscustomt=Iscustomt;
			x.ispresmooth=Ispresmooth;
			//x.closeradius=Closeradius;
			//x.isbinaryreconstruct=Isbinaryreconstruct;
			//x.binarymethod=Binarymethod;
			//x.isoriginreconstruct=Isoriginreconstruct;
			x.tolerate=Tolerate;
			//x.isinterpolate=Isinterpolate;
			//x.interpolatemethod=Interpolatemethod;

		}


		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ propertyName)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(propertyName));
		}

	};
	
	public ref class CWrapPathfinding
	{
	public:
		CWrapPathfinding();
		~CWrapPathfinding();
	protected:
		!CWrapPathfinding();


	public:
		CLRCrackParas^ crackparas;
		Bitmap^ bitmapforshow;
		Bitmap^ bitmapformarkshow;
		
	private:
		CCrackbyPathfinding* cracktool;

	private:
		Bitmap^ IplImagePointerToBitmap(const IplImage* mi);
		IplImage* BitmapToIplImagePointer(Bitmap^ bitmap);

	public:
		bool Calculate(const array<CLRLocation>^ clrlocArray,CLROptionalSET^ clroptionalset);//clrlocArray的行列坐标必须是相对于原始图像的坐标，且按照选取的顺序存储
		void Init(Bitmap^ bitmap);
		void LightSelected(int index);
		void MarkPoint(CLRLocation location,bool whetherclearbefore);
		void DeleteCrack(int index);
	
	};
}
