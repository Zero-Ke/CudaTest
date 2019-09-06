using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;
namespace Test_2013
{
    static class Program
    {
        static string path = @"img\";
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            ImageProcess.Test(3);
            bool bo = true;
            if (!bo)
            {
                TestShow("tan");//大图000010
            }
            else
            {
                TestShow("000010");
            }
            Console.ReadKey();
        }

        public static void TestShow(string imgName)
        {
            Bitmap orgBmp;
            orgBmp = Globles.NewReleasedBitmap(path + imgName + ".bmp");
            if (orgBmp == null)
            {
                Console.WriteLine("NULL");
                return;
            }
            CppColorRange[] cr=new CppColorRange[1];
            cr[0] = Init();
            //cr[1] = Init();
            ImageProcess.BinaryzationShow__GPU(orgBmp,cr,false);
            orgBmp.Save(path + imgName + "ShowGPU.bmp");
            orgBmp.Dispose();
        }
       static int i = 0;
        public static void Test(string imgName,bool isGpu=true)
        {
            i++;
            //Bitmap ratedBmp;
            Bitmap orgBmp;
            orgBmp = Globles.NewReleasedBitmap(path+ imgName + ".bmp");
            if (orgBmp==null)
            {
                Console.WriteLine("NULL");
                return;
            }
            //ratedBmp = Globles.NewBitmap(orgBmp.Width, orgBmp.Height);
            try
            {
                //ImageProcess.Ratezation(orgBmp, ratedBmp, isGpu);
                //ratedBmp.Save(path + isGpu.ToString()+ "_Rate.bmp");
                CppColorRange [] crA=new CppColorRange[1];
                CppColorRange cr = Init();
                crA[0]=cr;
                //ImageProcess.Binaryzation(ratedBmp, orgBmp, cr, isGpu);
                ImageProcess.BinaryzationsShow__CPU(orgBmp, cr, false);
                orgBmp.Save(path +i+ isGpu.ToString()+"_ShowCPU.bmp");
            }
            catch (Exception ex)
            {
                //ratedBmp.Dispose();
                //ratedBmp = null;
                Console.WriteLine(ex+"");
            }
            //ratedBmp.Dispose();
            //ratedBmp = null;
            orgBmp.Dispose();
            orgBmp = null;
        }
        public static CppColorRange Init()
        {
            CppColorRange cr;
            cr.blueDown = (byte)33;
            cr.blueUp = (byte)66;
            cr.blueUsed = true;
            cr.grayDown = (byte)50;
            cr.grayUp = (byte)255;
            cr.grayUsed = false;
            cr.greenDown = (byte)33;
            cr.greenUp = (byte)66;
            cr.greenUsed = true;
            cr.operation = (byte)0;
            cr.redDown = (byte)33;
            cr.redUp = (byte)66;
            cr.redUsed = false;
            return cr;
        }
    }
    
}
