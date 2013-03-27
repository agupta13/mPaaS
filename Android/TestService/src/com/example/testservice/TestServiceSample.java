package com.example.testservice;


import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class TestServiceSample extends Service {

	private static final String TAG = "TestServiceSample";
	
	//public static String CloudBridgeIp = "152.14.93.234";				//need to pickup from config file
    public static final int CloudBridgeManagementPort = 9006;			//need to pickup from config file
    public static String CloudBridgeIp = "ec2-50-16-128-208.compute-1.amazonaws.com";
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void onCreate() {
		Thread t = new Thread(new LaunchThread(this, CloudBridgeManagementPort));
	    t.start();
		Log.i(TAG, "Finished onCreate");
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.i(TAG, "Finished onStartCommand");
	    return START_NOT_STICKY;
	      
	}
	
	public void onDestroy() {
		Log.i(TAG, "Finished onDestroy");
	}

}
