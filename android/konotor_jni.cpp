#include "Konotor_cpp.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "support/CCNotificationCenter.h"

using namespace cocos2d;

const char *KonotorWrapperClass = "com/demach/konotor/cocos2dx/KonotorCocos2DXWrapper";
typedef void(*voidfunc)() ;

static voidfunc callbackFunctionHolder = 0;

#define KONOTOR_EVENT_UNREADCOUNTCHANGED "konotor_event_unreadcountchanged"

static void callStaticVoidMethod(const char *name, const char *signature, ...)
{
	cocos2d::JniMethodInfo methodInfo;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
											KonotorWrapperClass,
			                                name,
			                                signature) ) {
		va_list args;
		va_start(args, signature);
		methodInfo.env->CallStaticVoidMethodV(methodInfo.classID, methodInfo.methodID, args);
		va_end(args);

	}
}

static int callStaticIntMethod(const char *name, const char *signature, ...)
{
	cocos2d::JniMethodInfo methodInfo;

	jint ret = 0;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
											KonotorWrapperClass,
			                                name,
			                                signature) ) {
		va_list args;
		va_start(args, signature);
		ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, args);
		va_end(args);
	}
	return ret;
}

void KonotorCocos2dx::showFeedbackWidget()
{
	callStaticVoidMethod("showFeedbackWidget","()V");
}

void KonotorCocos2dx::setEmail(char *email)
{
	cocos2d::JniMethodInfo methodInfo;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
												KonotorWrapperClass,
												"setEmail",
												"(Ljava/lang/String;)V") ) {
		jstring stringArg = methodInfo.env->NewStringUTF(email);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
	}
}

void KonotorCocos2dx::setUsername(char *userName)
{
	cocos2d::JniMethodInfo methodInfo;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
												KonotorWrapperClass,
												"setUsername",
												"(Ljava/lang/String;)V") ) {
		jstring stringArg = methodInfo.env->NewStringUTF(userName);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
	}
}

void KonotorCocos2dx::setUserIdentifier(char * userIdentifier)
{
	cocos2d::JniMethodInfo methodInfo;
	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
												KonotorWrapperClass,
												"setUserIdentifier",
												"(Ljava/lang/String;)V") ) {
		jstring stringArg = methodInfo.env->NewStringUTF(userIdentifier);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
	}
}

/*
void KonotorCocos2dx::init(char *appId, char *appKey)
{
	cocos2d::JniMethodInfo methodInfo;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
												KonotorWrapperClass,
												"init",
												"(Ljava/lang/String;Ljava/lang/String;)V"
												) ) {
		jstring stringArg = methodInfo.env->NewStringUTF(appId);
		jstring stringArg2 = methodInfo.env->NewStringUTF(appKey);

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, stringArg2);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(stringArg2);
	}
}
*/

void KonotorCocos2dx::setUserMeta(char *key, char *value)
{
	cocos2d::JniMethodInfo methodInfo;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
												KonotorWrapperClass,
												"setUserMeta",
												"(Ljava/lang/String;Ljava/lang/String;)V"
												) ) {
		jstring stringArg = methodInfo.env->NewStringUTF(key);
		jstring stringArg2 = methodInfo.env->NewStringUTF(value);

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, stringArg2);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(stringArg2);
	}
}

void KonotorCocos2dx::update()
{
	callStaticVoidMethod("update","()V");
}

int KonotorCocos2dx::getUnreadCount()
{
	return callStaticIntMethod("getUnreadMessageCount","()I");
}


void KonotorCocos2dx::subscribeToUnreadCountChange(void (*callbackFunction) (void))
{
	callbackFunctionHolder = callbackFunction;
}

void KonotorCocos2dx::unSubscribeToUnreadCountChange()
{
	callbackFunctionHolder = 0;
}

extern "C" {
    void Java_com_demach_konotor_cocos2dx_KonotorCocos2DXWrapper_unreadCountChanged()
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(KONOTOR_EVENT_UNREADCOUNTCHANGED, NULL);
        if(callbackFunctionHolder!=NULL)
        	(*callbackFunctionHolder)();
    }
}
