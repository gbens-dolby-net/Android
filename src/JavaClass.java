package com.dolby.ddpexample;

import android.app.Activity;
import android.os.Looper;
import android.util.Log;
import java.io.File;

import android.media.dolby.DolbySurroundClient;
import android.media.dolby.DsClientSettings;
import android.media.dolby.IDsClientEvents;

public class JavaClass implements IDsClientEvents {
	private static final String TAG = JavaClass.class.getSimpleName();

	private static final int GAME_PROFILE = 2;

	private Activity mActivity;
	private DolbySurroundClient mDolbyClient;
	private DsClientSettings mProfileSettings;
	
	private boolean mDdpActive;
	private boolean mDialogEnhancerActive;
	private boolean mVolumeLevellerActive;
	private boolean mVirtualizerActive;
	
	private int mCurrentProfileId;
	private int mProfileCount;
	private String[] mProfileNames;

	public JavaClass(Activity currentActivity) {
		Log.i(TAG, "Constructor called with currentActivity = " + currentActivity);
		
		mActivity = currentActivity;

		// GKB: Needs to run on UI thread since DSClient requires a Handler()
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					Log.i(TAG, "Creating DS client on main thread");
					mDolbyClient = new DolbySurroundClient();
					mDolbyClient.bindToRemoteRunningService(mActivity);
					mDolbyClient.setEventListener(JavaClass.this);
					Log.i(TAG, "DS client created");
				}
				catch (Exception e) {
					Log.e(TAG, "Exception creating DS client: " + e.getMessage());
					e.printStackTrace();
				}
			}
		});
	}

	@Override
	public void onClientConnected() {
		Log.v(TAG, "Dolby client connected");
		try {
			mDolbyClient.setSelectedProfile(GAME_PROFILE);
			mDdpActive = mDolbyClient.getDolbySurroundOn();
			
			mProfileSettings = mDolbyClient.getProfileSettings(GAME_PROFILE);
			mDialogEnhancerActive = mProfileSettings.getDialogEnhancerOn();
			mVolumeLevellerActive = mProfileSettings.getVolumeLevellerOn();
			mVirtualizerActive = mProfileSettings.getVirtualizerOn();
			
			mCurrentProfileId = mDolbyClient.getSelectedProfile();
			mProfileCount = mDolbyClient.getProfileCount();
			mProfileNames = mDolbyClient.getProfileNames();
		}
		catch (Exception e) {
			Log.e(TAG, "Exception setting profile: " + e.getMessage());
			e.printStackTrace();
		}
	}

	@Override
	public void onClientDisconnected() {
		Log.w(TAG, "Dolby client disconnected");
	}

	@Override
	public void onDolbySurroundOn(boolean on) {
		Log.i(TAG, "Dolby surround is " + (on ? "on" : "off"));
	}

	@Override
	public void onProfileNameChanged(int profile, String name) {
		Log.d(TAG, "Dolby profile " + profile + " name changed. new name = " + name);
	}

	@Override
	public void onProfileSelected(int profile) {
		Log.i(TAG, "Dolby profile changed, and the new profile is " + profile);
	}

	@Override
	public void onProfileSettingsChanged(int profile) {
		Log.d(TAG, "Dolby profile " + profile + " settings changed.");
	}

	public boolean toggleDolbyDigitalPlus() {
		try {
			mDolbyClient.setDolbySurroundOn( ! mDdpActive);
			mDdpActive = ! mDdpActive;
			Log.v(TAG, "Toggling Dolby Digital Plus - " + mDdpActive);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to toggle Dolby Digital Plus");
			e.printStackTrace();
		}
		return mDdpActive;
	}

	/**
	 * @return Name of the newly-selected profile
	 */
	public String cycleDdpProfile() {
		String rtn = "Error";
		try {
			mCurrentProfileId = (mCurrentProfileId + 1) % mProfileCount;
			mDolbyClient.setSelectedProfile(mCurrentProfileId);
			rtn = mProfileNames[mCurrentProfileId];
			Log.v(TAG, "Cycling to profile " + mCurrentProfileId + ", " + rtn);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to cycle Ddp Profile");
			e.printStackTrace();
		}
		return rtn;
	}

	public boolean getDialogEnhancerOn() {
		boolean rtn = false;
		try {
			rtn = mProfileSettings.getDialogEnhancerOn();
			Log.v(TAG, "Dialog Enhancer is " + rtn);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to get dialog enhancer");
			e.printStackTrace();
		}
		return rtn;
	}

	public boolean toggleDialogEnhancer() {
		try {
			mProfileSettings.setDialogEnhancerOn( ! mDialogEnhancerActive);
			mDialogEnhancerActive = ! mDialogEnhancerActive;
			Log.v(TAG, "Toggling Dialog Enhancer - " + mDdpActive);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to toggle dialog enhancer");
			e.printStackTrace();
		}
		return mDialogEnhancerActive;
	}

	public boolean getVolumeLevellerOn() {
		boolean rtn = false;
		try {
			rtn = mProfileSettings.getVolumeLevellerOn();
			Log.v(TAG, "Volume Leveller is " + rtn);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to get volume leveller");
			e.printStackTrace();
		}
		return rtn;
	}

	public boolean toggleVolumeLeveller() {
		try {
			mProfileSettings.setVolumeLevellerOn( ! mVolumeLevellerActive);
			mVolumeLevellerActive = ! mVolumeLevellerActive;
			Log.v(TAG, "Toggling Volume Leveler - " + mDdpActive);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to toggle volume leveller");
			e.printStackTrace();
		}
		return mVolumeLevellerActive;
	}

	public boolean getVirtualizerOn() {
		boolean rtn = false;
		try {
			rtn = mProfileSettings.getVirtualizerOn();
			Log.v(TAG, "Virtualizer is " + rtn);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to get virtualizer");
			e.printStackTrace();
		}
		return rtn;
	}

	public boolean toggleVirtualizer() {
		try {
			mProfileSettings.setVirtualizerOn( ! mVirtualizerActive);
			mVirtualizerActive = ! mVirtualizerActive;
			Log.v(TAG, "Toggling Surround Virtualizer - " + mDdpActive);
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to toggle virtualizer");
			e.printStackTrace();
		}
		return mVirtualizerActive;
	}
}
