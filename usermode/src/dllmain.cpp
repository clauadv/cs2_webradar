// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.hpp"

bool main()
{
    LOG("usermode started \n");

    if (!exc::setup())
    {
        LOG_ERROR("failed to setup exception handler");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return true;
    }

    if (!mem::setup())
    {
        LOG_ERROR("failed to setup memory");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return true;
    }

    if (!sdk::setup())
    {
        LOG_ERROR("failed to setup sdk");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return true;
    }

    if (!schema::setup())
    {
        LOG_ERROR("failed to setup schema");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return true;
    }

    WSADATA wsa_data{};
    const auto wsa_startup = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wsa_startup != 0)
    {
        LOG_ERROR("failed to initialize winsock");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return true;
    }

    const auto ipv4_address = utils::get_ipv4_address();
    if (ipv4_address.empty())
    {
        LOG_ERROR("failed to get your ipv4 address, set it manually");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return true;
    }

    const auto formatted_address = format("ws://{}:22006/cs2_webradar", utils::get_ipv4_address());
    static auto web_socket = easywsclient::WebSocket::from_url(formatted_address);
    if (!web_socket)
    {
        LOG_ERROR("failed to connect to the web socket ('%s')", formatted_address.data());
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return true;
    }

    LOG("connected to the web socket ('%s')", formatted_address.data());

    auto start = std::chrono::system_clock::now();

    for (;;)
    {
        const auto now = std::chrono::system_clock::now();
        const auto duration = now - start;
        if (duration >= std::chrono::milliseconds(100))
        {
            start = now;

            f::run();

            // LOG("%s\n", f::m_data.dump().data());
            web_socket->send(f::m_data.dump());
        }

        web_socket->poll();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    system("pause");

    return true;
}