/**
 * 
 */
package com.example.testservice;



import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.graphics.Color;
import android.util.Log;

/**
 * @author Raghav
 *
 */
public class LEDThread extends Device{

	/* (non-Javadoc)
	 * @see java.lang.Runnable#run()
	 */
	
    
    public LEDThread(int dId, String dName, Context c){
    	super(dId, dName, c);
    }
    
    public void run() {
    	Log.e(TAG, "LEDPort: " + instructionPort);
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
			//in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
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
	        	buf = new StringBuffer();
	        	while((c = (char)in.read()) != '\n'){
	        		Log.i(TAG, "char: " + c);
	        		buf.append(c);
	        	}
	        	inputLine = buf.toString();
	        	Log.i(TAG, "Got from server: " + inputLine);
	        	
	        	if(inputLine == null || inputLine.isEmpty()){
	        		Log.e(TAG, "inputLine is malformed, continue");
	        		continue;
	        	}
	        	
	        	if(inputLine.equals("close")){
	        		Log.e(TAG, "Got socket close instruction from server, exiting");
	        		break;
	        	}
				
	            
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return;
			}
	        
	        String instruction = inputLine.substring(0, inputLine.indexOf(','));
			String port = inputLine.substring(inputLine.lastIndexOf(',')+1);
			Log.i(TAG, "Instruction: " + instruction + ", Port: " + port);
			Thread operation = null;
			if(instruction.equals("ON")){
				operation = new Thread(new performLEDOperation(0, 0, Integer.parseInt(port)));
				operation.start();
			}else{
				operation = new Thread(new performLEDOperation(0, 100, Integer.parseInt(port)));
				operation.start();
			}
			//buf.delete(0, buf.length());
        }
        
        try {
			clientSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }

    class performLEDOperation implements Runnable{
    	
    	private int onTime;
    	private int offTime;
    	private int ledPort;
    	private int whiteCode = Color.WHITE;
        private int idLED = 0;
        
    	performLEDOperation(int onT, int offT, int ledP){
    		onTime = onT;
    		offTime = offT;
    		ledPort = ledP;
    	}
    	
    	@Override
		public void run() {
			
			flashLED(whiteCode, onTime, offTime);
			PrintWriter out = null;
	    	Socket clientSocket = null;
			try {
		          clientSocket = new Socket(TestServiceSample.CloudBridgeIp, ledPort);
	        } catch (Exception e) {
	            Log.e(TAG, e.getMessage());
	            return;
	        }
		        
	        try {
				out = new PrintWriter(clientSocket.getOutputStream(), true);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return;
			}
	        if(offTime == 0)
	        	out.println("LED ON");
	        else
	        	out.println("LED OFF");

	        try {
				clientSocket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		     
		}
    	
	    public void flashLED(int code, int onTime, int offTime){
		    NotificationManager nm = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
		    Notification notif = new Notification();
		    notif.ledARGB = code;		
		    notif.flags |= Notification.FLAG_SHOW_LIGHTS;
		    notif.ledOnMS = onTime; 
		    notif.ledOffMS = offTime; 
		    nm.notify(idLED, notif);
		    Log.v(TAG, "Finished flashLED");
	    }
    }
    
}
