//
// Created by mboublil on 1/9/18.
//

#ifndef MUTILS_NETWORK_HPP
#define MUTILS_NETWORK_HPP

#include <memory>
#include <vector>
#include "Socket.hpp"
#include "ecs.hpp"
#include "AsyncSocket.hpp"


namespace mutils::net {

    struct test : public Serializable<test> {
        test() : Serializable(&test::testVal) {}

        std::string testVal {"salut lol !"};
    };

    struct Packet {
        int clientId;
        BinaryData data;
    };

    class Network : public futils::ISystem {
    public:
        explicit Network(bool isServer = false);
        void run(float) override;

        // For Client only
        void connect();
        void setHostname(std::string const &) {}

    private:
        void init();
        void monitorConnections();

    private:
        // Both
        std::unique_ptr<ITCPSocket> _tcpConnection;
        std::vector<std::unique_ptr<IUDPSocket>> _udpConnections;
        bool _isServer;
        int _phase{0};
        float _sumTime { 0 };

        // Server
        std::vector<int> _connections;
        std::unordered_map<int, AsyncSocket *> _connectionSocket;
        fd_set _rfds;
        std::thread _listenerThread;
        bool _serverShutdown { false };



    };
}

#endif //MUTILS_NETWORK_HPP