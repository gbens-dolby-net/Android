#include <stdlib.h>
#include <jni.h>
#include <android/log.h>

extern "C"
{

JavaVM*		java_vm;
jobject		JavaClass;
jmethodID	toggleDolbySurroundMethod;

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
	jclass cls_JavaClass	= jni_env->FindClass("org/example/ScriptBridge/JavaClass");
	jmethodID mid_JavaClass	= jni_env->GetMethodID(cls_JavaClass, "<init>", "(Landroid/app/Activity;)V");
	jobject obj_JavaClass	= jni_env->NewObject(cls_JavaClass, mid_JavaClass, obj_Activity);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] JavaClass object = %08x\n", __FUNCTION__, obj_JavaClass);

	// create a global reference to the JavaClass object and fetch method id(s)..
	JavaClass			= jni_env->NewGlobalRef(obj_JavaClass);
	toggleDolbySurroundMethod	= jni_env->GetMethodID(cls_JavaClass, "toggleDolbySurround", "()Ljava/lang/String;");
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] JavaClass global ref = %08x\n", __FUNCTION__, JavaClass);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] JavaClass method id = %08x\n", __FUNCTION__, toggleDolbySurroundMethod);
	
	return JNI_VERSION_1_6;		// minimum JNI version
}

const char* toggleDolbySurround()
{
	JNIEnv* jni_env = 0;
	java_vm->AttachCurrentThread(&jni_env, 0);

	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] called, attached to %08x\n", __FUNCTION__, jni_env);

	jstring str_cacheDir 	= (jstring)jni_env->CallObjectMethod(JavaClass, toggleDolbySurroundMethod);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] str_cacheDir = %08x\n", __FUNCTION__, jni_env);
	
	jsize stringLen = jni_env->GetStringUTFLength(str_cacheDir);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] stringLen = %i\n", __FUNCTION__, stringLen);
	
	char* rtnStr = new char[stringLen+1];
	
	const char* path = jni_env->GetStringUTFChars(str_cacheDir, 0);
	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] path = %s\n", __FUNCTION__, path);
	
	strcpy(rtnStr, path);

	__android_log_print(ANDROID_LOG_INFO, "JavaBridge", "[%s] return value is = %i\n", __FUNCTION__, rtnStr);
	
	return rtnStr;
}

} // extern "C"