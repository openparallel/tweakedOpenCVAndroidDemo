package com.example.tweakedopencvandroiddemo;

import java.io.InputStream;

import android.os.Bundle;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.text.Selection;
import android.text.Spannable;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends Activity {

	//use the JNI/NDK
		static {
			System.loadLibrary("opencv");
		}

		//for the following functions
		public native byte[] getSourceImage();
		public native boolean setSourceImage(int[] data, int w, int h);
		public native double doBenchmark(int method, boolean doOptimise);
		public native boolean fiveATime(int[] data1, int[] data2, int[] data3, int[] data4, int[] data5, int w, int h);
		
		private ImageView imageView;
		private int width, height;
		private String runTimeString; 
		public int[] getImageFromInputStream(InputStream is){
			Bitmap photo = BitmapFactory.decodeStream(is);
			
			int w = photo.getWidth();
			int h = photo.getHeight();
			int[] data = new int[w * h];

			//set the data with the pixels from the photo
			photo.getPixels(data, 0, w, 0, 0, w, h);
			photo.recycle();
			width = w;
			height = h;
			return data;
		}
		
		int getImageWidth(){
			return width;
		}
		
		int getImageHeight(){
			return height;
		}
		
		void zeroPointThreeMegaPixels(int typeOfOp){
			
			int data[];
			int w,h;
			InputStream is;
			
			double avgUnoptRuntime = 0;
			for(int i = 0; i < 20; i++){
				is = getResources().openRawResource(R.raw.la);
			
				data = getImageFromInputStream(is);
				w = getImageWidth();
				h = getImageHeight();
			
				setSourceImage(data, w, h);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
			
				is = getResources().openRawResource(R.raw.le);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.lg);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.lm);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.lq);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
			}
			
			Log.e("CaptainsLog", "The average unoptimised runtime is " + (avgUnoptRuntime/100));
			
			double avgOptRuntime = 0;
			for(int i = 0; i < 20; i++){
				is = getResources().openRawResource(R.raw.la);
			
				data = getImageFromInputStream(is);
				w = getImageWidth();
				h = getImageHeight();
			
				setSourceImage(data, w, h);
				avgOptRuntime += doBenchmark(typeOfOp,true);
			
				is = getResources().openRawResource(R.raw.le);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.lg);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.lm);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.lq);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
			}
			
			Log.e("CaptainsLog", "The average optimised runtime is " + (avgOptRuntime/100));
			Log.e("CaptainsLog", "The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
			
		}
		
		void OneMegaPixels(int typeOfOp){
			
			int data[];
			int w,h;
			InputStream is;
			
			double avgUnoptRuntime = 0;
			for(int i = 0; i < 20; i++){
				is = getResources().openRawResource(R.raw.ma);
			
				data = getImageFromInputStream(is);
				w = getImageWidth();
				h = getImageHeight();
			
				setSourceImage(data, w, h);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
			
				is = getResources().openRawResource(R.raw.me);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.mg);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.mm);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.mq);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
			}
			
			Log.e("CaptainsLog", "The average unoptimised runtime is " + (avgUnoptRuntime/100));
			
			double avgOptRuntime = 0;
			for(int i = 0; i < 20; i++){
				is = getResources().openRawResource(R.raw.ma);
			
				data = getImageFromInputStream(is);
				w = getImageWidth();
				h = getImageHeight();
			
				setSourceImage(data, w, h);
				avgOptRuntime += doBenchmark(typeOfOp,true);
			
				is = getResources().openRawResource(R.raw.me);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.mg);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.mm);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.mq);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
			}
			
			Log.e("CaptainsLog", "The average optimised runtime is " + (avgOptRuntime/100));
			Log.e("CaptainsLog", "The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
			
		}
		

		void FiveMegaPixels(int typeOfOp){
		
		int data[];
		int w,h;
		InputStream is;
		
		double avgUnoptRuntime = 0;
		for(int i = 0; i < 20; i++){
			is = getResources().openRawResource(R.raw.ha);
		
			data = getImageFromInputStream(is);
			w = getImageWidth();
			h = getImageHeight();
		
			setSourceImage(data, w, h);
			avgUnoptRuntime += doBenchmark(typeOfOp,false);
		
			is = getResources().openRawResource(R.raw.he);
			data = getImageFromInputStream(is);
			avgUnoptRuntime += doBenchmark(typeOfOp,false);
			
			is = getResources().openRawResource(R.raw.hg);
			data = getImageFromInputStream(is);
			avgUnoptRuntime += doBenchmark(typeOfOp,false);
			
			is = getResources().openRawResource(R.raw.hm);
			data = getImageFromInputStream(is);
			avgUnoptRuntime += doBenchmark(typeOfOp,false);
			
			is = getResources().openRawResource(R.raw.hq);
			data = getImageFromInputStream(is);
			avgUnoptRuntime += doBenchmark(typeOfOp,false);
			
		}
		
		Log.e("CaptainsLog", "The average unoptimised runtime is " + (avgUnoptRuntime/100));
		
		double avgOptRuntime = 0;
		for(int i = 0; i < 20; i++){
			is = getResources().openRawResource(R.raw.ha);
		
			data = getImageFromInputStream(is);
			w = getImageWidth();
			h = getImageHeight();
		
			setSourceImage(data, w, h);
			avgOptRuntime += doBenchmark(typeOfOp,true);
		
			is = getResources().openRawResource(R.raw.he);
			data = getImageFromInputStream(is);
			avgOptRuntime += doBenchmark(typeOfOp,true);
			
			is = getResources().openRawResource(R.raw.hg);
			data = getImageFromInputStream(is);
			avgOptRuntime += doBenchmark(typeOfOp,true);
			
			is = getResources().openRawResource(R.raw.hm);
			data = getImageFromInputStream(is);
			avgOptRuntime += doBenchmark(typeOfOp,true);
			
			is = getResources().openRawResource(R.raw.hq);
			data = getImageFromInputStream(is);
			avgOptRuntime += doBenchmark(typeOfOp,true);
			
		}
		
		Log.e("CaptainsLog", "The average optimised runtime is " + (avgOptRuntime/100));
		Log.e("CaptainsLog", "The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
		
	}


		void EightMegaPixels(int typeOfOp){
			int data[];
			int w,h;
			InputStream is;
		
			double avgUnoptRuntime = 0;
			for(int i = 0; i < 20; i++){
				is = getResources().openRawResource(R.raw.vha);
				data = getImageFromInputStream(is);
				w = getImageWidth();
				h = getImageHeight();
				
				setSourceImage(data, w, h);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.vhe);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.vhg);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.vhm);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
				
				is = getResources().openRawResource(R.raw.vhq);
				data = getImageFromInputStream(is);
				avgUnoptRuntime += doBenchmark(typeOfOp,false);
			}
		
			Log.e("CaptainsLog", "The average unoptimised runtime is " + (avgUnoptRuntime/100));
			
			double avgOptRuntime = 0;
			for(int i = 0; i < 20; i++){
				is = getResources().openRawResource(R.raw.vha);
				
				data = getImageFromInputStream(is);
				w = getImageWidth();
				h = getImageHeight();

				setSourceImage(data, w, h);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.vhe);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.vhg);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.vhm);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
				is = getResources().openRawResource(R.raw.vhq);
				data = getImageFromInputStream(is);
				avgOptRuntime += doBenchmark(typeOfOp,true);
				
			}
			Log.e("CaptainsLog", "The average optimised runtime is " + (avgOptRuntime/100));
			Log.e("CaptainsLog", "The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
			
		}

		protected void showToUser(String newContent){
			
			TextView textView = (TextView)findViewById(R.id.textView1);
			textView.append(newContent);
			textView.setSelected(true); 
			Spannable text = (Spannable) textView.getText(); 
			Selection.setSelection(text, text.length()); 
			
		}
		
		@Override
		protected void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);
			setContentView(R.layout.activity_main);
		
			final Button benchmarkButton = (Button)findViewById(R.id.button1);
			final ImageView imageView =  (ImageView)findViewById(R.id.imageView1);
			TextView textView = (TextView)findViewById(R.id.textView1);
			
			runTimeString = "";
			textView.setText(runTimeString);
			
			benchmarkButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v){

				new Thread(new Runnable() {
			        public void run() {

						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "640x480 (0.3 mpx resolution)");
						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "convert 32 -> 16");
						Log.e("CaptainsLog", "----------------------------");
						
						showToUser("****************************\n");
						showToUser("640x480 (0.3 mpx resolution)");
						
						zeroPointThreeMegaPixels(0);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(1);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(2);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(3);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(4);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(5);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(6);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(7);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(8);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(9);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(10);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Gaussian Blur");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(11);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Sobel Filter");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(12);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Edge Detection");
						Log.e("CaptainsLog", "----------------------------");
						zeroPointThreeMegaPixels(13);
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						
						
						
						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "1280x960 (1 mpx resolution)");
						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "convert 32 -> 16");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(0);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(1);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(2);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(3);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(4);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(5);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(6);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(7);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(8);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(9);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(10);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Gaussian Blur");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(11);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Sobel Filter");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(12);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Edge Detection");
						Log.e("CaptainsLog", "----------------------------");
						OneMegaPixels(13);
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						
						
						
						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "2560x1920 (5 mpx resolution)");
						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "convert 32 -> 16");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(0);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(1);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(2);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(3);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(4);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(5);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(6);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(7);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(8);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(9);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(10);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Gaussian Blur");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(11);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Sobel Filter");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(12);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Edge Detection");
						Log.e("CaptainsLog", "----------------------------");
						FiveMegaPixels(13);
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						
						
						
						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "3264x2448 (8 mpx resolution)");
						Log.e("CaptainsLog", "****************************");
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "convert 32 -> 16");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(0);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(1);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(2);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(3);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(4);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(5);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh (8-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(6);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "binary thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(7);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "trunc thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(8);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(9);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "to-zero thresh inv (16-bit)");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(10);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Gaussian Blur");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(11);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Sobel Filter");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(12);
						Log.e("CaptainsLog", "----------------------------");
						Log.e("CaptainsLog", "Edge Detection");
						Log.e("CaptainsLog", "----------------------------");
						EightMegaPixels(13);
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						Log.e("CaptainsLog", "");
						
						byte[] resultData = getSourceImage();
						Bitmap resultPhoto = BitmapFactory.decodeByteArray(resultData, 0, resultData.length);
						imageView.setImageBitmap(resultPhoto);
			        }
				}).start();
			}});
		}
}