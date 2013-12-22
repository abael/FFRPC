#ifndef _FF_FFSCENE_LUA_H_
#define _FF_FFSCENE_LUA_H_

//#include <assert.h>
//#include <string>
using namespace std;

#include "server/ffscene.h"
#include "base/log.h"
#include "base/smart_ptr.h"
#include "server/db_mgr.h"
#include "server/ffjson_tool.h"

namespace ff
{
#define FFSCENE_PYTHON "FFSCENE_PYTHON"

#define MOD_NAME            "ffext"
#define VERIFY_CB_NAME      "ff_session_verify"
#define ENTER_CB_NAME       "ff_session_enter"
#define OFFLINE_CB_NAME     "ff_session_offline"
#define LOGIC_CB_NAME       "ff_session_logic"
#define TIMER_CB_NAME       "ff_timer_callback"
#define DB_QUERY_CB_NAME    "ff_db_query_callback"
#define SCENE_CALL_CB_NAME  "ff_scene_call"
#define CALL_SERVICE_RETURN_MSG_CB_NAME "ff_scene_call_return_msg"

class fflua_t;
class ffjson_tool_t;

class ffscene_lua_t: public ffscene_t, task_dispather_i
{
public:
    arg_helper_t    m_arg_helper;
public:
    ffscene_lua_t();
    ~ffscene_lua_t();
    int open(arg_helper_t& arg_helper);
    int close();
    string reload(const string& name_);
    void pylog(int level, const string& mod_, const string& content_);
    //! 判断某个service是否存在
    bool is_exist(const string& service_name_);
    ffslot_t::callback_t* gen_verify_callback();
        
    ffslot_t::callback_t* gen_enter_callback();
        
    ffslot_t::callback_t* gen_offline_callback();
        
    ffslot_t::callback_t* gen_logic_callback();
    ffslot_t::callback_t* gen_scene_call_callback();
    //! 定时器接口
    int once_timer(int timeout_, uint64_t id_);
    
    ffslot_t::callback_t* gen_db_query_callback(long callback_id_);
    
    //! 创建数据库连接
    long connect_db(const string& host_);
    void db_query(long db_id_,const string& sql_, long callback_id_);
    vector<vector<string> > sync_db_query(long db_id_,const string& sql_);
    void call_service(const string& name_, long cmd_, const string& msg_, long id_);
    void bridge_call_service(const string& group_name_, const string& name_, long cmd_, const string& msg_, long id_);
    void call_service_return_msg(ffreq_t<scene_call_msg_t::out_t>& req_, long id_);
    
    fflua_t& get_fflua(){ return *m_fflua; }
    
    //! 线程间传递消息
    void post_task(const string& func_name, const ffjson_tool_t& task_args, long callback_id);
    void post_task_impl(const string& func_name, const ffjson_tool_t& task_args, long callback_id);
public:
    fflua_t*        m_fflua;
    db_mgr_t        m_db_mgr;
};

}

#endif