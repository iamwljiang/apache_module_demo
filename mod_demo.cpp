//author: jiangwenlong@pipi.cn 2012-05
//desc  : search module 

//apache header
#include "ap_config.h"
#include "httpd.h"
#include "http_config.h"
#include "http_request.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_main.h"
#include "http_log.h"

//user header
#include <string>
#include <vector>
#define MAX_PATH 256

extern "C" module AP_MODULE_DECLARE_DATA demo_module;

typedef struct 
{
	char str_demo_arg1[MAX_PATH];
	int  int_demo_arg2;
}demo_config;

//if you need global variable,you can add variable in here:
//static unsigned int global_real_request = 0;

extern "C" {
static void module_init(apr_pool_t *pchild, server_rec *s)
{

	demo_config* cfg = (demo_config*)ap_get_module_config(s->module_config,&demo_module);
	//what you want do any init thing at apache child inited 
	ap_log_error(APLOG_MARK,APLOG_INFO,0,s,"module inited,str arg:%s,int arg:%d",cfg->str_demo_arg1,cfg->int_demo_arg2);
}

//process http request
static int demo_handler(request_rec *r)
{
	std::string servername = r->server->server_hostname;
	/*
	example of demo module will only allow process this uri:
	/demo 
	*/
	if(strncmp(r->uri,"/demo",5) != 0)
		return DECLINED;
	
	ap_log_error(APLOG_MARK,APLOG_INFO,0,r->server,"servername:%s,args:%s",r->server->server_hostname,r->args);

	demo_config* cfg = NULL;
	//this get virtual config
	//cfg=(demo_config*)ap_get_module_config(r->per_dir_config,&demo_module);
	cfg = (demo_config*)ap_get_module_config(r->server->module_config,&demo_module);
	
	//module process:user only care here
	std::string result = "This is apache demo module\n";
	result += "Client ip:";
	result += r->connection->remote_ip ;
	result += "\ndemo str:"  ;
	result += cfg->str_demo_arg1;
	char demo_int_buf[MAX_PATH]="";
	sprintf(demo_int_buf,",demo int:%d\n",cfg->int_demo_arg2);
	result += demo_int_buf;

	//output processed result
	ap_set_content_type(r, "text/html; charset=utf-8");
	ap_set_content_length(r, result.size());
	ap_rwrite(result.c_str(),result.size(),r);
	ap_rflush(r);
	return OK;
}

//definition command function
static const char* set_mod_string(cmd_parms *parms, void *mconfig, const char *arg)
{
	demo_config* config = (demo_config*)ap_get_module_config(parms->server->module_config,&demo_module);
	if(strcmp(parms->cmd->name,"demo_str") == 0){
		strcpy(config->str_demo_arg1,arg);
	}else if (strcmp(parms->cmd->name,"demo_int") == 0){
		config->int_demo_arg2 = atoi(arg);
	}

	return NULL;
}

//allocate memory for config struct
static void *create_dir_config(apr_pool_t *p,char *dir)
{
	return apr_pcalloc(p, sizeof(demo_config));
}

static void *create_server_config(apr_pool_t *p, server_rec *s)
{
	return apr_pcalloc(p, sizeof(demo_config));
}

//definition command
static const command_rec mod_cmds[] = 
{

	AP_INIT_TAKE1("demo_str",(cmd_func)set_mod_string,NULL,OR_ALL/*argument position*/,"demo first argument"),
	AP_INIT_TAKE1("demo_int",(cmd_func)set_mod_string,NULL,OR_ALL,"demo second argument"),

	{NULL}
};

//register hooks
static void register_hooks(apr_pool_t *p)
{
	ap_hook_child_init(module_init,NULL,NULL,APR_HOOK_REALLY_FIRST);
	ap_hook_handler(demo_handler,NULL,NULL,APR_HOOK_MIDDLE);
}

//definition module
module AP_MODULE_DECLARE_DATA demo_module = 
{
	STANDARD20_MODULE_STUFF,
	create_dir_config,
	NULL,
	create_server_config,
	NULL,
	mod_cmds,
	register_hooks
};

};//end extern c
