package com.example.testservice;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

public class AccelerometerThread extends Device implements SensorEventListener {

	private final SensorManager mSensorManager;
    private final Sensor mAccelerometer;
    private float x = 0;		//what to set as default values?
    private float y = 0;
    private float z = 0;
    
    public AccelerometerThread(int dId, String dName, Context c){
    	super(dId, dName, c);
    	mSensorManager = (SensorManager)context.getSystemService(Context.SENSOR_SERVICE);
    	mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
    	//mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_FASTEST);			
    }
    
	@Override
	public void run() {
		if(mAccelerometer == null){
			Log.e(TAG, "No Acclerometer on device");
			return;
		}
		
		Log.i(TAG, "minDelay: " + mAccelerometer.getMinDelay());
		Log.e(TAG, "AccelerometerPort: " + instructionPort);
    	InputStreamReader in = null;
    	PrintWriter out = null;
    	Socket clientSocket = null;
    	String inputLine = null;
    	
        try {
        	clientSocket = new Socket(TestServiceSample.CloudBridgeIp, instructionPort);
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }
        
        try {
			out = new PrintWriter(clientSocket.getOutputStream(), true);
			in = new InputStreamReader(clientSocket.getInputStream());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}
        
        out.println("Hi");
        StringBuffer buf = new StringBuffer();

        while(true){
	        try {
	        	char c;
	        	while((c = (char)in.read()) != '\n'){
	        		Log.i(TAG, "char: " + c);
	        		buf.append(c);
	        	}
	        	inputLine = buf.toString();
	        	if(inputLine.equals("close"))
	        		break;
				Log.i(TAG, "Got from server: " + inputLine);
				mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_FASTEST);
				for(long i = 0; i < 10000000; i++);
					
				out.println(x + "," + y + "," + z);
				
				mSensorManager.unregisterListener(this);
			} catch (IOException e) {
				e.printStackTrace();
				return;
			}
        }
        
        try {
			clientSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        

	}

	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		x = event.values[0];
		y = event.values[0];
		z = event.values[0];
	}

}
