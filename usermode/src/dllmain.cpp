// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.hpp"

bool main()
{
    LOG("usermode started \n");

    if (!src::source2::c_base_sdk::setup())
    {
        LOG_ERROR("failed to setup cs2");
        this_thread::sleep_for(chrono::seconds(5));
        return true;
    }

    // @easywsclient
    WSADATA wsa_data{};
    const auto wsa_startup = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wsa_startup != 0)
    {
        LOG_ERROR("failed to initialize winsock");
        this_thread::sleep_for(chrono::seconds(5));
        return true;
    }

    const auto ipv4_address = utils::get_ipv4_address();
    if (ipv4_address.empty())
    {
        LOG_ERROR("failed to get your ipv4 address, set it manually");
        this_thread::sleep_for(chrono::seconds(5));
        return true;
    }

    const auto formatted_address = format("ws://{}:22006/cs2_webradar", utils::get_ipv4_address());
    static auto web_socket = easywsclient::WebSocket::from_url(formatted_address);
    if (!web_socket)
    {
        LOG_ERROR("failed to connect to the web socket ('%s')", formatted_address.data());
        this_thread::sleep_for(chrono::seconds(5));
        return true;
    }

    LOG("connected to the web socket ('%s')", formatted_address.data());

    auto start = chrono::system_clock::now();

    for (;;)
    {
        const auto now = chrono::system_clock::now();
        const auto duration = now - start;
        if (duration >= chrono::milliseconds(100))
        {
            start = now;

            m_features.run();

            // LOG("%s", m_features.get_data().dump().data());
            web_socket->send(m_features.get_data().dump());
        }

        web_socket->poll();
    }

    system("pause");

    return true;
}