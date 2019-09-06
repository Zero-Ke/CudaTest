using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;


class ImageProcess
{
    [DllImport("CPPDll.dll", EntryPoint = "#1", CallingConvention = CallingConvention.Cdecl)]
    public static extern int Test( int a);
    [DllImport("CPPDll.dll", EntryPoint = "#2", CallingConvention = CallingConvention.Cdecl)]
    static extern int CppRatezationGPU(IntPtr bitmapFrom, IntPtr bitmapTo, int width, int height);
    [DllImport("CPPDll.dll", EntryPoint = "#3", CallingConvention = CallingConvention.Cdecl)]
    static extern int CppBinaryzationGPU(ref CppColorRange range, IntPtr bitmapFrom, IntPtr bitmapTo, int width, int height);
    [DllImport("CPPDll.dll", EntryPoint = "#4", CallingConvention = CallingConvention.Cdecl)]
    static extern int CppBinaryzationCPU(ref CppColorRange range, IntPtr bitmapFr, IntPtr bitmapTo, int width, int height);
    [DllImport("CPPDll.dll", EntryPoint = "#5", CallingConvention = CallingConvention.Cdecl)]
    static extern int CppRatezationCPU(IntPtr bitmapFrom, IntPtr bitmapTo, int width, int height);
    [DllImport("CPPDll.dll", EntryPoint = "#7", CallingConvention = CallingConvention.Cdecl)]
    static extern int CppBinaryShowAll(IntPtr bitmapFr, IntPtr bitmapTo, int width, int height);
    [DllImport("CPPDll.dll", EntryPoint = "#8", CallingConvention = CallingConvention.Cdecl)]
    static extern int CppBinaryShowOne(IntPtr bitmapFr, IntPtr bitmapTo, int width, int height);
    [DllImport("CPPDll.dll", EntryPoint = "#9", CallingConvention = CallingConvention.Cdecl)]
    static extern int CppBinaryShow([In,Out] CppColorRange[] range, int count, IntPtr bitmapFr, IntPtr bitmapTo, int width, int height, int isall);
    /// <summary>
    /// 将图像修改为包含选中信息的图像.
    /// </summary>
    /// <param name="target">源和目标图像</param>
    /// <param name="threshold">颜色范围</param>
    public static void Ratezation(Bitmap bmpfr, Bitmap bmpto,bool isGPU)
    {
        try
        {
            
            System.Diagnostics.Stopwatch s = new System.Diagnostics.Stopwatch();
            int width = bmpto.Width;
            int height = bmpto.Height;
            BitmapData fromdata = bmpfr.LockBits(new Rectangle(0, 0, bmpfr.Width, bmpfr.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            BitmapData todata = bmpto.LockBits(new Rectangle(0, 0, bmpto.Width, bmpto.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            if(isGPU)
            {
                s.Restart();
                CppRatezationGPU(fromdata.Scan0, todata.Scan0, width, height);
                Console.WriteLine("=============GPU Ratezation耗时：" + s.ElapsedMilliseconds);
            }
            else
            {
                s.Restart();
                CppRatezationCPU(fromdata.Scan0, todata.Scan0, width, height);
                Console.WriteLine("=============CPU Ratezation耗时：" + s.ElapsedMilliseconds);
            }
            bmpfr.UnlockBits(fromdata);
            bmpto.UnlockBits(todata);
        }
        catch (Exception ex) { Console.WriteLine(ex + ""); }
    }
    public static void Binaryzation(Bitmap bmpfr, Bitmap bmpto, CppColorRange color,bool isGPU)
    {
        try
        {
            System.Diagnostics.Stopwatch s = new System.Diagnostics.Stopwatch();
            if (bmpfr == null || bmpto == null)
                return;
            BitmapData fromdata = bmpfr.LockBits(new Rectangle(0, 0, bmpfr.Width, bmpfr.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            BitmapData todata = bmpto.LockBits(new Rectangle(0, 0, bmpto.Width, bmpto.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            IntPtr fromptr = fromdata.Scan0;
            IntPtr toprt = todata.Scan0;
            if(isGPU)
            {
                s.Restart();
                CppBinaryzationGPU(ref color, fromptr, toprt, bmpto.Width, bmpto.Height);
                Console.WriteLine("=============GPU Binaryzation 耗时：" + s.ElapsedMilliseconds);
            }
            else
            {
                s.Restart();
                CppBinaryzationCPU(ref color, fromptr, toprt, bmpto.Width, bmpto.Height);
                Console.WriteLine("=============CPU Binaryzation 耗时：" + s.ElapsedMilliseconds);
            }
            bmpfr.UnlockBits(fromdata);
            bmpto.UnlockBits(todata);
        }
        catch (Exception ex) { Console.WriteLine(ex+""); }
    }
    /// <summary>
    /// 将图像修改为包含选中信息的图像.
    /// </summary>
    /// <param name="target">源和目标图像</param>
    /// <param name="threshold">颜色范围</param>
    public static void BinaryzationsShow__CPU(Bitmap target, CppColorRange ccr, bool isAll = false)
    {
        try
        {
            System.Diagnostics.Stopwatch s = new System.Diagnostics.Stopwatch();
           
            Bitmap temp = new Bitmap(target);//(Bitmap)target.Clone();
            BitmapData tempData = temp.LockBits(new Rectangle(0, 0, target.Width, target.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            BitmapData targetData = target.LockBits(new Rectangle(0, 0, target.Width, target.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            IntPtr tempPtr = tempData.Scan0;
            IntPtr targetPtr = targetData.Scan0;
            s.Restart();
            CppRatezationCPU(tempPtr, tempPtr, target.Width, target.Height);
            if (isAll)
            {
                CppBinaryzationCPU(ref ccr, tempPtr, targetPtr, target.Width, target.Height);
                CppBinaryShowAll(targetPtr, targetPtr, target.Width, target.Height);
            }
            else
            {
                //当前抽色信息
                CppBinaryzationCPU(ref ccr, tempPtr, targetPtr, target.Width, target.Height);
                CppBinaryShowOne(targetPtr, targetPtr, target.Width, target.Height);
            }
            Console.WriteLine("BinaryzationsShow__CPU 耗时 "+s.ElapsedMilliseconds);
            temp.UnlockBits(tempData);
            target.UnlockBits(targetData);
            temp.Dispose();
        }
        catch (Exception ex) { Console.WriteLine(ex.Message); }
    }
    public static void BinaryzationShow__GPU(Bitmap target, CppColorRange[] ccr, bool isAll = false)
    {
        try
        {
            System.Diagnostics.Stopwatch s = new System.Diagnostics.Stopwatch();
            if (ccr.Length == 0) return;
            Bitmap temp = new Bitmap(target);
            BitmapData tempData = temp.LockBits(new Rectangle(0, 0, target.Width, target.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            BitmapData targetData = target.LockBits(new Rectangle(0, 0, target.Width, target.Height), ImageLockMode.ReadWrite, Globles.PixelFormat);
            IntPtr tempPtr = tempData.Scan0;
            IntPtr targetPtr = targetData.Scan0;
            s.Restart();
            CppBinaryShow(ccr, ccr.Length, tempPtr, targetPtr, target.Width, target.Height, isAll?1:0);
            Console.WriteLine("CppBinaryShow 耗时"+s.ElapsedMilliseconds);
            temp.UnlockBits(tempData);
            target.UnlockBits(targetData);
            temp.Dispose();
        }
        catch (Exception ex) { Console.WriteLine(ex.Message); }
    }
}

public struct CppColorRange
{
    public int operation;
    public bool redUsed;
    public byte redUp;
    public byte redDown;
    public bool greenUsed;
    public byte greenUp;
    public byte greenDown;
    public bool blueUsed;
    public byte blueUp;
    public byte blueDown;
    public bool grayUsed;
    public byte grayUp;
    public byte grayDown;
}
class Globles
{
    public static PixelFormat PixelFormat = PixelFormat.Format32bppArgb;
    /// <summary>
    /// 从文件中读入图片，并关闭过程中使用的文件。若不关闭文件，此图片文件需要在读入的图片对象被释放后才能重新使用。
    /// </summary>
    /// <param name="p">文件名称</param>
    /// <returns>读入的图片</returns>
    public static Bitmap NewReleasedBitmap(string p)
    {
        Bitmap rtn = null;
        System.IO.FileStream fs = null;
        try
        {
            if (!File.Exists(p))
            {
                return null;
            }
            fs = File.OpenRead(p);
            rtn = new Bitmap(fs);
            //fs.Close();
            return rtn;
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex + "");
            if (rtn != null)
                rtn.Dispose();
            return null;
        }
        finally
        {
            if (fs != null)
                fs.Close();
        }
    }
    /// <summary>
    /// 新建一默认像素格式的全黑的图片文件
    /// </summary>
    /// <param name="wd">图片宽度</param>
    /// <param name="ht">图片高度</param>
    /// <returns></returns>
    public static Bitmap NewBitmap(int wd, int ht)
    {
        Bitmap bmp = new Bitmap(wd, ht, PixelFormat.Format32bppArgb);
        Graphics g = Graphics.FromImage(bmp);
        g.Clear(Color.Black);
        g.Dispose();
        return bmp;
    }
    public static Bitmap CloneBmp(Bitmap bmp)
    {
        return (Bitmap)bmp.Clone();
    }
}