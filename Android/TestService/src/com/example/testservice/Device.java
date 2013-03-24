package com.example.testservice;

import android.content.Context;

public abstract class Device implements Runnable{

	protected int devId;
	protected String devName;
	protected int instructionPort;
	protected Context context;
	protected static String TAG;
	
	protected Device(int dId, String dName, Context c){
		
		devId = dId;
		devName = dName;
		context = c;
		TAG = devName;
	}
	
	public int getInstructionPort() {
		return instructionPort;
	}

	public void setInstructionPort(int instructionPort) {
		this.instructionPort = instructionPort;
	}
	
	public int getDevId() {
		return devId;
	}
	
	public String getDevName() {
		return devName;
	}
}
