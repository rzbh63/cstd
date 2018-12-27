#include <stdio.h>
#include "cJSON.hpp"


cJSON* test_cjson_gen() {
  cJSON * root = cJSON_CreateObject();
  cJSON * item = cJSON_CreateObject();
  cJSON * next = cJSON_CreateObject();

  cJSON_AddItemToObject(root, "rc", cJSON_CreateNumber(0));//���ڵ������
  cJSON_AddItemToObject(root, "operation", cJSON_CreateString("CALL"));
  cJSON_AddItemToObject(root, "service", cJSON_CreateString("telephone"));
  cJSON_AddItemToObject(root, "text", cJSON_CreateString("��绰������"));
  cJSON_AddItemToObject(root, "semantic", item);//root�ڵ������semantic�ڵ�
  cJSON_AddItemToObject(item, "slots", next);//semantic�ڵ������item�ڵ�
  cJSON_AddItemToObject(next, "name", cJSON_CreateString("����"));//���name�ڵ�
  return root;
}


int test_cjson()
{
  cJSON* root = test_cjson_gen();
  printf("%s\n", cJSON_Print(root));
  cJSON_Delete(root);
  return 0;
}