package org.example.ScriptBridge;

import android.app.Activity;
import android.os.Looper;
import android.util.Log;
import java.io.File;

import android.media.dolby.DolbySurroundClient;
import android.media.dolby.IDsClientEvents;

public class JavaClass implements IDsClientEvents {
	private static final String TAG = JavaClass.class.getSimpleName();

	private Activity mActivity;
	private DolbySurroundClient mDolbyClient;
	private boolean mDolbySurroundActive;

	public JavaClass(Activity currentActivity) {
		Log.i("JavaClass", "Constructor called with currentActivity = " + currentActivity);
		
		mActivity = currentActivity;

//		mDolbyClient = new DolbySurroundClient();
//		mDolbyClient.bindToRemoteRunningService(mActivity);
//		mDolbyClient.setEventListener(this);

		// GKB: Needs to run on UI thread since DSClient requires a Handler()
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					Log.i("JavaClass", "Creating DS client on main thread");
					mDolbyClient = new DolbySurroundClient();
					mDolbyClient.bindToRemoteRunningService(mActivity);
					mDolbyClient.setEventListener(JavaClass.this);
					Log.i("JavaClass", "DS client created");
				}
				catch (Exception e) {
					Log.e(TAG, "Exception creating DS client: " + e.getMessage());
					e.printStackTrace();
				}
			}
		});
	}

	//	// we could of course do this straight from native code using JNI, but this is an example so.. ;)
	//	public String getActivityCacheDir()
	//	{
	//		// calling Context.getCacheDir();
	//		// http://developer.android.com/reference/android/content/Context.html#getCacheDir()
	//		//
	//		final File cacheDir = mActivity.getCacheDir();
	//		final String path = cacheDir.getPath();
	//		Log.i(TAG, "getActivityCacheDir returns = " + path);
	//		return path;
	//	}

	public String toggleDolbySurround() {
		try {
			mDolbyClient.setDolbySurroundOn( ! mDolbySurroundActive);
			mDolbySurroundActive = ! mDolbySurroundActive;
		}
		catch (Exception e) {
			Log.e(TAG, "Excep trying to toggle Dolby Surround");
			e.printStackTrace();
		}
		return mDolbySurroundActive ? "DolbySurround is ON" : "DolbySurround is off";
	}

	@Override
	public void onClientConnected() {
		Log.v(TAG, "Dolby client connected");
		try {
			mDolbyClient.setSelectedProfile(2);
			mDolbySurroundActive = mDolbyClient.getDolbySurroundOn();
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
}
