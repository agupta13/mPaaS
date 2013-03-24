package com.example.testservice;


import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.Socket;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.StringTokenizer;

import android.content.Context;
import android.util.Log;

public class LaunchThread implements Runnable {

	private static final String TAG = "LaunchThread";
	private int CloudBridgeManagementPort;
	private Context context;
	private static final String androidPlatform = "android";
	private List<Device> deviceList = new ArrayList<Device>();
	
	public LaunchThread(Context c, int port){
		context = c;
		CloudBridgeManagementPort = port;
		deviceList.add(new LEDThread(02, "LED", context));
		deviceList.add(new AccelerometerThread(03, "Accelerometer", context));
	}
	
	@Override
	public void run() {
    	InputStreamReader in = null;
    	PrintWriter out = null;
    	Socket clientSocket = null;
    	
        try {
          clientSocket = new Socket(TestServiceSample.CloudBridgeIp, CloudBridgeManagementPort);
        } catch (Exception e) {
            Log.e(TAG, e.getMessage());
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
        
        StringBuffer mgmtData = new StringBuffer(getLocalIpAddress() +  "," + androidPlatform);
        for(Device d: deviceList){
        	mgmtData.append(d.getDevId());
        	mgmtData.append(",");
        }
        mgmtData.deleteCharAt(mgmtData.length() - 1);
        
        Log.i(TAG, "Sending Data: " + mgmtData.toString());
        out.println(mgmtData.toString());
        
        
        String inputLine = null;
        StringBuffer buf = new StringBuffer();
        try {
        	char c;
        	while((c = (char)in.read()) != '\n'){
        		Log.i(TAG, "char: " + c);
        		buf.append(c);
        	}
        	inputLine = buf.toString();
			Log.i(TAG, "Got from server: " + inputLine);
            
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}
        
        try {
			clientSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        StringTokenizer str = new StringTokenizer(inputLine, ",");
        String port = null;		
        int index = 0;
       
        while(str.hasMoreTokens() && index < deviceList.size()){
        	port = str.nextToken();
        	Log.e(TAG, "Device port: " + port);
        	Device dev = deviceList.get(index);
        	dev.setInstructionPort(Integer.parseInt(port));
        	Thread t = new Thread(dev);
        	t.start();
        	index++;
        }
    }
		
	private String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress()) 
                    	return inetAddress.getHostAddress().toString();
                }
            }
        } catch (SocketException ex) {
            Log.e(TAG, "getLocalIpAddress Exception " + ex.getMessage());
        }
        return null;
    }

}
