//
// Created by mboublil on 1/18/18.
//

#include "AsyncSocket.hpp"

namespace mutils::net {

    std::mutex _mutexCv;
    std::vector<int> _fds;
    std::condition_variable _cv;
    std::unordered_map<SOCKET, Actions> _action;

    AsyncSocket::AsyncSocket(ITCPSocket *sock)  : _sock(sock) {
        _clientThread = std::thread([this]() {
            std::unique_lock<std::mutex> lk(_mutexCv);
            while (!_stop) {

                _cv.wait(lk, [this]() {
                    {
                        for (auto it = _fds.begin(); it != _fds.end(); it++) {
                            if (*it == _sock->getSocket()) {
                                _fds.erase(it);
                                return true;
                            }
                        }
                    }
                    return false;
                });

                switch (_action[_sock->getSocket()]) {
                    case Actions::READ:
                        readContent();
                        _action.erase(_sock->getSocket());
                        break ;
                    case Actions::WRITE:
                        writeContent();
                        _action.erase(_sock->getSocket());
                        break ;
                    default:
                        break ;
                }
//                _cv.notify_all();
//                analyzeSockets();
            }
        });
    }

    AsyncSocket::~AsyncSocket() {
        _stop = true;
        _fds.push_back(_sock->getSocket());
        _action[_sock->getSocket()] = Actions::NONE;
        _clientThread.join();
    }

    Header AsyncSocket::getHeader() {
        std::size_t size;
        std::uint16_t type;

        _sock->receiveData(reinterpret_cast<char *>(&type), sizeof(std::uint16_t));
        _sock->receiveData(reinterpret_cast<char *>(&size), sizeof(std::size_t));

        return {type, size};
    }

    void AsyncSocket::readContent() {
        Header hdr = getHeader();
        DataInfos infos = _sock->receiveData(nullptr, hdr.size); BinaryData data;

        data.hdr = hdr;
        data._dataStr = infos.data;
        received(_sock->getSocket(), data);
    }

    void AsyncSocket::writeContent() {
        auto data = toSend();

        auto size = _sock->sendData(data.second._dataStr.c_str(), data.second.hdr.size);
        std::cout << "DATA SENT. SIZE: " << size << " BUT REAL SIZE: " << data.second.hdr.size << std::endl;
    }

}
