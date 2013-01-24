package com.example.tweakedopencvandroiddemo;

import java.io.InputStream;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.text.NoCopySpan.Concrete;
import android.text.Selection;
import android.text.Spannable;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ScrollView;
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
		
		
		private Handler handler = new Handler() {
			@Override
				public void handleMessage(Message newContent) {
					TextView textView = (TextView)findViewById(R.id.textView1);
					Bundle resBundle = (Bundle)newContent.obj;
					
					
					textView.append(resBundle.getString("status").toString()+"\n");
					
					
					
					textView.setSelected(true); 
					Spannable text = (Spannable) textView.getText(); 
					Selection.setSelection(text, text.length()); 
					
					ScrollView scrollView = (ScrollView)findViewById(R.id.scrollView1);
					scrollView.fullScroll(View.FOCUS_DOWN);
				}
		};
		
		
		private Handler imageDisplayHandler = new Handler() {
			@Override
				public void handleMessage(Message newContent) {
					Log.e("Captains Log", "Oh boy... a new piccy!");
					ImageView imageView = (ImageView)findViewById(R.id.imageView1);
					
					byte[] resultData = getSourceImage();
					Bitmap resultPhoto = BitmapFactory.decodeByteArray(resultData, 0, resultData.length);
					
					imageView.setImageBitmap(resultPhoto);
				
				}
		};
			
		
		@Override
		protected void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);
			setContentView(R.layout.activity_main);
						
			final Button benchmarkButton = (Button)findViewById(R.id.button1);
			ImageView imageView =  (ImageView)findViewById(R.id.imageView1);
			TextView textView = (TextView)findViewById(R.id.textView1);
			
			textView.setText("");
			
			benchmarkButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v){

				new Thread(new Runnable() {
					public void passANewImage(){
						Message myMessage=new Message();
						Bundle resBundle = new Bundle();
						resBundle.putString("status", "");
						myMessage.obj=resBundle;
						imageDisplayHandler.sendMessage(myMessage);
					}
					
					public void passMessage(String message){
						Message myMessage=new Message();
						Bundle resBundle = new Bundle();
						resBundle.putString("status", message);
						myMessage.obj=resBundle;
						handler.sendMessage(myMessage);
					}
					
					public void report(String message){
						Log.e("CaptainsLog", message);
						passMessage(message);
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
							passANewImage();
							
							is = getResources().openRawResource(R.raw.le);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.lg);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.lm);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.lq);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
						}
						
						report("The average unoptimised runtime is " + (avgUnoptRuntime/100));
						
						double avgOptRuntime = 0;
						for(int i = 0; i < 20; i++){
							is = getResources().openRawResource(R.raw.la);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.le);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.lg);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.lm);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.lq);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
						}
						
						report("The average optimised runtime is " + (avgOptRuntime/100));
						report("The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
						
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
							passANewImage();
							
							is = getResources().openRawResource(R.raw.me);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.mg);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.mm);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.mq);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
						}
						
						report("The average unoptimised runtime is " + (avgUnoptRuntime/100));
						
						double avgOptRuntime = 0;
						for(int i = 0; i < 20; i++){
							is = getResources().openRawResource(R.raw.ma);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.me);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.mg);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.mm);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.mq);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
						}
						
						report("The average optimised runtime is " + (avgOptRuntime/100));
						report("The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
						
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
						passANewImage();
						
						is = getResources().openRawResource(R.raw.he);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgUnoptRuntime += doBenchmark(typeOfOp,false);
						passANewImage();
						
						is = getResources().openRawResource(R.raw.hg);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgUnoptRuntime += doBenchmark(typeOfOp,false);
						passANewImage();
						
						is = getResources().openRawResource(R.raw.hm);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgUnoptRuntime += doBenchmark(typeOfOp,false);
						passANewImage();
						
						is = getResources().openRawResource(R.raw.hq);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgUnoptRuntime += doBenchmark(typeOfOp,false);
						passANewImage();
					}
					
					report("The average unoptimised runtime is " + (avgUnoptRuntime/100));
					
					double avgOptRuntime = 0;
					for(int i = 0; i < 20; i++){
						is = getResources().openRawResource(R.raw.ha);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgOptRuntime += doBenchmark(typeOfOp,true);
						passANewImage();
						
						is = getResources().openRawResource(R.raw.he);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgOptRuntime += doBenchmark(typeOfOp,true);
						passANewImage();
						
						is = getResources().openRawResource(R.raw.hg);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgOptRuntime += doBenchmark(typeOfOp,true);
						passANewImage();
						
						is = getResources().openRawResource(R.raw.hm);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgOptRuntime += doBenchmark(typeOfOp,true);
						passANewImage();
						
						is = getResources().openRawResource(R.raw.hq);
						data = getImageFromInputStream(is);
						w = getImageWidth();
						h = getImageHeight();
						setSourceImage(data, w, h);
						avgOptRuntime += doBenchmark(typeOfOp,true);
						passANewImage();
					}
					
					report("The average optimised runtime is " + (avgOptRuntime/100));
					report("The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
					
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
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhe);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhg);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhm);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhq);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgUnoptRuntime += doBenchmark(typeOfOp,false);
							passANewImage();
						}
					
						report("The average unoptimised runtime is " + (avgUnoptRuntime/100));
						
						double avgOptRuntime = 0;
						for(int i = 0; i < 20; i++){
							is = getResources().openRawResource(R.raw.vha);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhe);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhg);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhm);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
							
							is = getResources().openRawResource(R.raw.vhq);
							data = getImageFromInputStream(is);
							w = getImageWidth();
							h = getImageHeight();
							setSourceImage(data, w, h);
							avgOptRuntime += doBenchmark(typeOfOp,true);
							passANewImage();
						}
						report("The average optimised runtime is " + (avgOptRuntime/100));
						report("The speedup is " + (avgUnoptRuntime/100)/(avgOptRuntime/100));
						
					}
					
			        public void run() {

						report("****************************");
						report("640x480 (0.3 mpx resolution)");
						report("****************************");
						report("----------------------------");
						report("convert 32 -> 16");
						report("----------------------------");						
						zeroPointThreeMegaPixels(0);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(1);
						report("----------------------------");
						report("binary thresh inv (8-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(2);
						report("----------------------------");
						report("trunc thresh (8-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(3);
						report("----------------------------");
						report("to-zero thresh (8-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(4);
						report("----------------------------");
						report("to-zero thresh inv (8-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(5);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(6);
						report("----------------------------");
						report("binary thresh inv (16-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(7);
						report("----------------------------");
						report("trunc thresh (16-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(8);
						report("----------------------------");
						report("to-zero thresh (16-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(9);
						report("----------------------------");
						report("to-zero thresh inv (16-bit)");
						report("----------------------------");
						zeroPointThreeMegaPixels(10);
						report("----------------------------");
						report("Gaussian Blur");
						report("----------------------------");
						zeroPointThreeMegaPixels(11);
						report("----------------------------");
						report("Sobel Filter");
						report("----------------------------");
						zeroPointThreeMegaPixels(12);
						report("----------------------------");
						report("Edge Detection");
						report("----------------------------");
						zeroPointThreeMegaPixels(13);
						report("");
						report("");
						report("");
						
						
						
						report("****************************");
						report("1280x960 (1 mpx resolution)");
						report("****************************");
						report("----------------------------");
						report("convert 32 -> 16");
						report("----------------------------");
						OneMegaPixels(0);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						OneMegaPixels(1);
						report("----------------------------");
						report("binary thresh inv (8-bit)");
						report("----------------------------");
						OneMegaPixels(2);
						report("----------------------------");
						report("trunc thresh (8-bit)");
						report("----------------------------");
						OneMegaPixels(3);
						report("----------------------------");
						report("to-zero thresh (8-bit)");
						report("----------------------------");
						OneMegaPixels(4);
						report("----------------------------");
						report("to-zero thresh inv (8-bit)");
						report("----------------------------");
						OneMegaPixels(5);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						OneMegaPixels(6);
						report("----------------------------");
						report("binary thresh inv (16-bit)");
						report("----------------------------");
						OneMegaPixels(7);
						report("----------------------------");
						report("trunc thresh (16-bit)");
						report("----------------------------");
						OneMegaPixels(8);
						report("----------------------------");
						report("to-zero thresh (16-bit)");
						report("----------------------------");
						OneMegaPixels(9);
						report("----------------------------");
						report("to-zero thresh inv (16-bit)");
						report("----------------------------");
						OneMegaPixels(10);
						report("----------------------------");
						report("Gaussian Blur");
						report("----------------------------");
						OneMegaPixels(11);
						report("----------------------------");
						report("Sobel Filter");
						report("----------------------------");
						OneMegaPixels(12);
						report("----------------------------");
						report("Edge Detection");
						report("----------------------------");
						OneMegaPixels(13);
						report("");
						report("");
						report("");
						
						
						
						report("****************************");
						report("2560x1920 (5 mpx resolution)");
						report("****************************");
						report("----------------------------");
						report("convert 32 -> 16");
						report("----------------------------");
						FiveMegaPixels(0);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						FiveMegaPixels(1);
						report("----------------------------");
						report("binary thresh inv (8-bit)");
						report("----------------------------");
						FiveMegaPixels(2);
						report("----------------------------");
						report("trunc thresh (8-bit)");
						report("----------------------------");
						FiveMegaPixels(3);
						report("----------------------------");
						report("to-zero thresh (8-bit)");
						report("----------------------------");
						FiveMegaPixels(4);
						report("----------------------------");
						report("to-zero thresh inv (8-bit)");
						report("----------------------------");
						FiveMegaPixels(5);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						FiveMegaPixels(6);
						report("----------------------------");
						report("binary thresh inv (16-bit)");
						report("----------------------------");
						FiveMegaPixels(7);
						report("----------------------------");
						report("trunc thresh (16-bit)");
						report("----------------------------");
						FiveMegaPixels(8);
						report("----------------------------");
						report("to-zero thresh (16-bit)");
						report("----------------------------");
						FiveMegaPixels(9);
						report("----------------------------");
						report("to-zero thresh inv (16-bit)");
						report("----------------------------");
						FiveMegaPixels(10);
						report("----------------------------");
						report("Gaussian Blur");
						report("----------------------------");
						FiveMegaPixels(11);
						report("----------------------------");
						report("Sobel Filter");
						report("----------------------------");
						FiveMegaPixels(12);
						report("----------------------------");
						report("Edge Detection");
						report("----------------------------");
						FiveMegaPixels(13);
						report("");
						report("");
						report("");
						
						
						
						report("****************************");
						report("3264x2448 (8 mpx resolution)");
						report("****************************");
						report("----------------------------");
						report("convert 32 -> 16");
						report("----------------------------");
						EightMegaPixels(0);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						EightMegaPixels(1);
						report("----------------------------");
						report("binary thresh inv (8-bit)");
						report("----------------------------");
						EightMegaPixels(2);
						report("----------------------------");
						report("trunc thresh (8-bit)");
						report("----------------------------");
						EightMegaPixels(3);
						report("----------------------------");
						report("to-zero thresh (8-bit)");
						report("----------------------------");
						EightMegaPixels(4);
						report("----------------------------");
						report("to-zero thresh inv (8-bit)");
						report("----------------------------");
						EightMegaPixels(5);
						report("----------------------------");
						report("binary thresh (8-bit)");
						report("----------------------------");
						EightMegaPixels(6);
						report("----------------------------");
						report("binary thresh inv (16-bit)");
						report("----------------------------");
						EightMegaPixels(7);
						report("----------------------------");
						report("trunc thresh (16-bit)");
						report("----------------------------");
						EightMegaPixels(8);
						report("----------------------------");
						report("to-zero thresh (16-bit)");
						report("----------------------------");
						EightMegaPixels(9);
						report("----------------------------");
						report("to-zero thresh inv (16-bit)");
						report("----------------------------");
						EightMegaPixels(10);
						report("----------------------------");
						report("Gaussian Blur");
						report("----------------------------");
						EightMegaPixels(11);
						report("----------------------------");
						report("Sobel Filter");
						report("----------------------------");
						EightMegaPixels(12);
						report("----------------------------");
						report("Edge Detection");
						report("----------------------------");
						EightMegaPixels(13);
						report("");
						report("");
						report("");
						passANewImage();
//						byte[] resultData = getSourceImage();
//						Bitmap resultPhoto = BitmapFactory.decodeByteArray(resultData, 0, resultData.length);
//						imageView.setImageBitmap(resultPhoto);
			        }
				}).start();
			}});
		}
}