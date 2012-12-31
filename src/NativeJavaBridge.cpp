#include <stdlib.h>
#include <jni.h>
#include <android/log.h>

extern "C"
{

JavaVM*		java_vm;
jobject		JavaClass;

jmethodID	toggleDdpMethod;

jmethodID	getDialogEnhancerOnMethod;
jmethodID	getVolumeLevellerOnMethod;
jmethodID	getVirtualizerOnMethod;
jmethodID	toggleDialogEnhancerMethod;
jmethodID	toggleVolumeLevellerMethod;
jmethodID	toggleVirtualizerMethod;

jmethodID	cycleDdpProfileMethod;
jmethodID	getCurrentProfileNameMethod;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	// use __android_log_print for logcat debugging...
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] Creating java link vm = %08x\n", __FUNCTION__, vm);
	java_vm = vm;

	// attach our thread to the java vm; obviously it's already attached but this way we get the JNIEnv..
	JNIEnv* jni_env = 0;
	java_vm->AttachCurrentThread(&jni_env, 0);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] JNI Environment is = %08x\n", __FUNCTION__, jni_env);

	// first we try to find our main activity..
	jclass cls_Activity		= jni_env->FindClass("com/unity3d/player/UnityPlayer");
	jfieldID fid_Activity	= jni_env->GetStaticFieldID(cls_Activity, "currentActivity", "Landroid/app/Activity;");
	jobject obj_Activity	= jni_env->GetStaticObjectField(cls_Activity, fid_Activity);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] Current activity = %08x\n", __FUNCTION__, obj_Activity);

	// create a JavaClass object...
	jclass cls_JavaClass	= jni_env->FindClass("com/dolby/ddpexample/JavaClass");
	jmethodID mid_JavaClass	= jni_env->GetMethodID(cls_JavaClass, "<init>", "(Landroid/app/Activity;)V");
	jobject obj_JavaClass	= jni_env->NewObject(cls_JavaClass, mid_JavaClass, obj_Activity);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] JavaClass object = %08x\n", __FUNCTION__, obj_JavaClass);

	// create a global reference to the JavaClass object and fetch method id(s)..
	JavaClass					= jni_env->NewGlobalRef(obj_JavaClass);
	toggleDdpMethod				= jni_env->GetMethodID(cls_JavaClass, "toggleDolbyDigitalPlus", "()Z");

	getDialogEnhancerOnMethod	= jni_env->GetMethodID(cls_JavaClass, "getDialogEnhancerOn", "()Z");
	getVolumeLevellerOnMethod	= jni_env->GetMethodID(cls_JavaClass, "getVolumeLevellerOn", "()Z");
	getVirtualizerOnMethod		= jni_env->GetMethodID(cls_JavaClass, "getVirtualizerOn", "()Z");
	toggleDialogEnhancerMethod	= jni_env->GetMethodID(cls_JavaClass, "toggleDialogEnhancer", "()Z");
	toggleVolumeLevellerMethod	= jni_env->GetMethodID(cls_JavaClass, "toggleVolumeLeveller", "()Z");
	toggleVirtualizerMethod		= jni_env->GetMethodID(cls_JavaClass, "toggleVirtualizer", "()Z");

	cycleDdpProfileMethod		= jni_env->GetMethodID(cls_JavaClass, "cycleDdpProfile", "()Ljava/lang/String;");
	getCurrentProfileNameMethod	= jni_env->GetMethodID(cls_JavaClass, "getCurrentProfileName", "()Ljava/lang/String;");
	
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "Method: %s", __FUNCTION__);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "JavaClass global ref = %08x\n", JavaClass);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "toggleDdpMethod method id = %08x\n", toggleDdpMethod);
	
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "getDialogEnhancerOnMethod method id = %08x\n", getDialogEnhancerOnMethod);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "getVolumeLevellerOnMethod method id = %08x\n", getVolumeLevellerOnMethod);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "getVirtualizerOnMethod method id = %08x\n", getVirtualizerOnMethod);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "toggleDialogEnhancerMethod method id = %08x\n", toggleDialogEnhancerMethod);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "toggleVolumeLevellerMethod method id = %08x\n", toggleVolumeLevellerMethod);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "toggleVirtualizerMethod method id = %08x\n", toggleVirtualizerMethod);

	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "cycleDdpProfileMethod method id = %08x\n", cycleDdpProfileMethod);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "getCurrentProfileNameMethod method id = %08x\n", getCurrentProfileNameMethod);

	return JNI_VERSION_1_6;		// minimum JNI version
}

const char* callMethodWithStringReturn(jmethodID methodID)
{
	JNIEnv* jni_env = 0;
	java_vm->AttachCurrentThread(&jni_env, 0);

	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] called, attached to %08x\n", __FUNCTION__, jni_env);

	jstring str_jniResult 	= (jstring)jni_env->CallObjectMethod(JavaClass, methodID);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] str_jniResult = %08x\n", __FUNCTION__, str_jniResult);
	
	jsize stringLen = jni_env->GetStringUTFLength(str_jniResult);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] stringLen = %i\n", __FUNCTION__, stringLen);
	
	char* rtnStr = new char[stringLen+1];
	
	const char* jni_utfChars = jni_env->GetStringUTFChars(str_jniResult, 0);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] jni_utfChars = %s\n", __FUNCTION__, jni_utfChars);
	
	strcpy(rtnStr, jni_utfChars);

	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] return value is = %i\n", __FUNCTION__, rtnStr);
	return rtnStr;
}

const char* cycleDdpProfile()
{
	return callMethodWithStringReturn(cycleDdpProfileMethod);
}

const char* getCurrentProfileName()
{
	return callMethodWithStringReturn(getCurrentProfileNameMethod);
}

const bool callMethodWithBoolReturn(jmethodID methodID)
{
	JNIEnv* jni_env = 0;
	java_vm->AttachCurrentThread(&jni_env, 0);

	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] called, attached to %08x\n", __FUNCTION__, jni_env);

	jboolean java_rtn 	= (jboolean)jni_env->CallObjectMethod(JavaClass, methodID);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] java_rtn = %08x\n", __FUNCTION__, java_rtn);

	return (bool)(java_rtn != JNI_FALSE);
}

const bool toggleDolbyDigitalPlus()
{
	return callMethodWithBoolReturn(toggleDdpMethod);
}

const bool getDialogEnhancerOn()
{
	return callMethodWithBoolReturn(getDialogEnhancerOnMethod);
}

const bool getVolumeLevellerOn()
{
	return callMethodWithBoolReturn(getVolumeLevellerOnMethod);
}

const bool getVirtualizerOn()
{
	return callMethodWithBoolReturn(getVirtualizerOnMethod);
}

const bool toggleDialogEnhancer()
{
	return callMethodWithBoolReturn(toggleDialogEnhancerMethod);
}

const bool toggleVolumeLeveller()
{
	return callMethodWithBoolReturn(toggleVolumeLevellerMethod);
}

const bool toggleVirtualizer()
{
	return callMethodWithBoolReturn(toggleVirtualizerMethod);
}

} // extern "C"