#include <evpp/http/http_server.h>
#include <evpp/httpc/request.h>
#include <evpp/httpc/response.h>

#include "winmain-inl.h"
#include "appimp.h"
#include <czmq.h>


zlog_category_t* g_server_cat;
zlog_category_t* g_server_cat_xjhm;
zlog_category_t* g_server_cat_hldq;
ConfigPara g_conf;

void DefaultHandler(evpp::EventLoop* loop,
                    const evpp::http::ContextPtr& ctx,
                    const evpp::http::HTTPSendResponseCallback& cb) {
    std::stringstream oss;
    oss << "please post url: http://http://222.190.240.226:38888/njits" << "\n";
    ctx->AddResponseHeader("Content-Type", "application/octet-stream");
    ctx->AddResponseHeader("Server", "evpp");
    cb(oss.str());
}

void NjitsHandler(evpp::EventLoop* loop,
                  const evpp::http::ContextPtr& ctx,
                  const evpp::http::HTTPSendResponseCallback& cb) {
    //1.验证消息头

    appManager *appMan = appManager::instance(); ;

    RecHead headers;
    char *reqinfo;
    if(!(appMan->verifyHeaders((char *)ctx->body().ToString().c_str(),headers)))
    {
       reqinfo = appMan->pkg->ReplyJson(headers,"fail");
       zlog_error(g_server_cat," rely  json failed \n");
    }
    else //true
    {
        if(strcmp("njitsxjhmcz",(char*)headers.szUserName) == 0)
        {
            //发送给server,做分析处理
            if(g_conf.GetDBused() == 1)
            {
                zstr_send (appMan->push, (char *)ctx->body().ToString().c_str());
            }
            zlog_info(g_server_cat_xjhm,"\n---------------------------------- \n  Handler post receives \n  body:%s",(char *)ctx->body().ToString().c_str());
        }
        else if(strcmp("njitshldqwh",(char*)headers.szUserName) == 0)
        {
            zlog_info(g_server_cat_hldq,"\n---------------------------------- \n  Handler post receives \n  body:%s",(char *)ctx->body().ToString().c_str());
        }else
        {
            zlog_info(g_server_cat,"\n---------------------------------- \n  Handler post receives \n  body:%s",(char *)ctx->body().ToString().c_str());
        }

        //重新组装body消息
        char * rebuildJson = appMan->rebuildTrajData((char *)ctx->body().ToString().c_str(),headers);

       // 2.发送kafka
        if(g_conf.GetKafkaServerused() == 1)
        {
            if(appMan->pushKafkaData((char *)rebuildJson) == 0)
            {
                    zlog_info(g_server_cat,"%s pushKafkaData,success\n",(char*)headers.szUserName);
            }
            else
            {
                    zlog_error(g_server_cat,"%s pushKafkaData,failed",(char*)headers.szUserName);
            }
        }
       reqinfo = appMan->pkg->ReplyJson(headers,"ok");
       zlog_info(g_server_cat,"rely  json success \n");
    }
    //3.发送返回信息
   ctx->AddResponseHeader("Content-Type", "application/json");
   ctx->AddResponseHeader("Server", "njitsbigData");
   cb(reqinfo);
}

int main(int argc, char* argv[]) {

    //app init
    appManager  *appMan = appManager::instance();
    appMan->init();
    //http
    int port1 =g_conf.GetHttpPort1();
    int port2 =g_conf.GetHttpPort2();

    std::vector<int> ports = {port1, port2};
    int thread_num = g_conf.GetHttpThreadNum();
    evpp::http::Server server(thread_num);
    server.SetThreadDispatchPolicy(evpp::ThreadDispatchPolicy::kIPAddressHashing);
    server.RegisterDefaultHandler(&DefaultHandler);
    server.RegisterHandler("/njits", &NjitsHandler);
    server.RegisterHandler("/echo",
                           [](evpp::EventLoop* loop,
                              const evpp::http::ContextPtr& ctx,
                              const evpp::http::HTTPSendResponseCallback& cb) {
        cb(ctx->body().ToString()); }
    );
    server.Init(ports);
    server.Start();
    while (!server.IsStopped()) {
        usleep(1);
    }

    return 0;
}
