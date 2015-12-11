﻿#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H


#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

//#include "Game/session.hpp"
#include "hf_types.h"

using namespace hf_types;

#define       TCP_BUFFER_SIZE           1024

typedef struct _STR_Package
{
    char data[TCP_BUFFER_SIZE];
    unsigned int roleid;
}STR_Package;

using  boost::asio::ip::tcp;

class TCPConnection:public boost::enable_shared_from_this<TCPConnection>
{
public:
    typedef boost::shared_ptr<TCPConnection>    Pointer;
    ~TCPConnection();
    /**
     * @brief Create
     * @param io_service
     * @return
     */
    static Pointer  Create( boost::asio::io_service &io_service)
    {
        return Pointer(new TCPConnection(io_service));
    }

//    bool operator == ( TCPConnection &conn)
//    {
//      Pointer ptr = shared_from_this();
//       return socket().native_handle() == conn->socket().native_handle();
//    }
    /**
     * @brief Write_all   将指定长度的数据发送出去
     * @param buff        要发送数据的存储去
     * @param size        要发送数据的长度
     * @return  int ,        实际发送出去的字节数或者异常
     */
    int   Write_all( void *buff, int size );


    void CallBack_Read_Some(const boost::system::error_code &code,size_t size);


    void CallBack_Write( const boost::system::error_code &code, size_t transfferd);

    //判断玩家是否登录角色
    bool JudgePlayerLogin(/*SessionMgr::SessionPointer smap, */TCPConnection::Pointer conn, hf_uint8 flag);

    /**
     * @brief Start
     */
    void                    Start();
    /**
     * @brief socket
     * @return
     */
    tcp::socket&        socket()   { return m_socket;}

    boost::mutex          m_write_lock;
    boost::mutex          m_read_lock;

private:
    TCPConnection(boost::asio::io_service &io);

    hf_uint32 currentIndex;
    hf_char                 m_buf[TCP_BUFFER_SIZE];
    STR_Package             m_pack;

    void                    ReadSome();

    tcp::socket             m_socket;

    //发送缓冲区
    char                    m_send_buf[TCP_BUFFER_SIZE];
    //
    //设置读写锁，保证通过网络传输的包的完整性。
    //一旦发生异常，及时将锁释放，否则会造成死锁
    //


};

#endif // TCPCONNECTION_H
