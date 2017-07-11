#include <iostream>

#include "com_seaboat_bytecode_ByteCodeEncryptor.h"
#include "jni.h"
#include <jvmti.h>
#include <jni_md.h>


void encode(char *str)
{
	unsigned int m = strlen(str);
	for (int i = 0; i < m; i++)
	{
		//str[i] = ((str[i] - 97)*k) - ((str[i] - 97)*k) / q*q + 97;
		str[i] = str[i] + k;
	}

}

void decode(char *str)
{
	unsigned int m = strlen(str);
	//int k2 = (q + 1) / k;
	for (int i = 0; i < m; i++)
	{
		//str[i] = ((str[i] - 97)*k2) - ((str[i] - 97)*k2) / q*q + 97;
		str[i] = str[i] - k;
	}
}


extern"C" JNIEXPORT jbyteArray JNICALL
Java_com_seaboat_bytecode_ByteCodeEncryptor_encrypt(JNIEnv * env, jclass cla, jbyteArray text)
{
	char* dst = (char*)env->GetByteArrayElements(text, 0);
	encode(dst);
	env->SetByteArrayRegion(text, 0, strlen(dst), (jbyte *)dst);
	return text;
}


void JNICALL ClassDecryptHook(
	jvmtiEnv *jvmti_env,
	JNIEnv* jni_env,
	jclass class_being_redefined,
	jobject loader,
	const char* name,
	jobject protection_domain,
	jint class_data_len,
	const unsigned char* class_data,
	jint* new_class_data_len,
	unsigned char** new_class_data
	)
{
	*new_class_data_len = class_data_len;
	jvmti_env->Allocate(class_data_len, new_class_data);

	unsigned char* _data = *new_class_data;

	if (name&&strncmp(name, "com/seaboat/", 12) == 0) {
		for (int i = 0; i < class_data_len; i++)
		{
			_data[i] = class_data[i];
		}
		decode((char*)_data);
	}
	else {
		for (int i = 0; i < class_data_len; i++)
		{
			_data[i] = class_data[i];
		}
	}

}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved)
{

	jvmtiEnv *jvmti;
	//Create the JVM TI environment(jvmti)
	jint ret = vm->GetEnv((void **)&jvmti, JVMTI_VERSION);
	if (JNI_OK != ret)
	{
		printf("ERROR: Unable to access JVMTI!\n");
		return ret;
	}
	jvmtiCapabilities capabilities;
	(void)memset(&capabilities, 0, sizeof(capabilities));

	capabilities.can_generate_all_class_hook_events = 1;
	capabilities.can_tag_objects = 1;
	capabilities.can_generate_object_free_events = 1;
	capabilities.can_get_source_file_name = 1;
	capabilities.can_get_line_numbers = 1;
	capabilities.can_generate_vm_object_alloc_events = 1;

	jvmtiError error = jvmti->AddCapabilities(&capabilities);
	if (JVMTI_ERROR_NONE != error)
	{
		printf("ERROR: Unable to AddCapabilities JVMTI!\n");
		return error;
	}

	jvmtiEventCallbacks callbacks;
	(void)memset(&callbacks, 0, sizeof(callbacks));

	callbacks.ClassFileLoadHook = &ClassDecryptHook;
	error = jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
	if (JVMTI_ERROR_NONE != error) {
		printf("ERROR: Unable to SetEventCallbacks JVMTI!\n");
		return error;
	}

	error = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
	if (JVMTI_ERROR_NONE != error) {
		printf("ERROR: Unable to SetEventNotificationMode JVMTI!\n");
		return error;
	}

	return JNI_OK;
}

