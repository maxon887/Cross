package com.cross;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.Toast;

public class MessageBox {
	
	static Thread thread = new Thread() {
		@Override
		public void run() {
			Looper.prepare();
	        mHandler = new Handler() {
	            public void handleMessage(Message msg) {
	            	Toast.makeText(context, message, Toast.LENGTH_LONG).show();
	            }
	        };
	        Looper.loop();
		};
	};
	
	static public Handler mHandler = null;
	static Context context;
	static String message;
	
	static {
		thread.start();
	}
	
	public static void ShowMessage(Context cont, String msg) {
		//wait for looper
		while(mHandler == null) { }
		
		context = cont;
		message = msg;
        mHandler.sendEmptyMessage(0);
        try {
			Thread.sleep(4000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
