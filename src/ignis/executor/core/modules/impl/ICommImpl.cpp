
#include "ICommImpl.h"

using namespace ignis::executor::core::modules::impl;

ICommImpl::ICommImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

ICommImpl::~ICommImpl() {}

std::string ICommImpl::createGroup() {
    IGNIS_TRY()
        IGNIS_LOG(info) << "Comm: creating group";
        char port_name[MPI_MAX_PORT_NAME];
        MPI::Open_port(MPI::INFO_NULL, port_name);
        auto handle = std::shared_ptr<std::string>(new std::string(port_name), [](const std::string *group_name) {
            MPI::Close_port(group_name->c_str());//Close port on context clear
            delete group_name;
        });
        IGNIS_LOG(info) << "Comm: group created on " << port_name;
        executor_data->setVariable<std::shared_ptr<std::string>>("server", handle);
        return port_name;
    IGNIS_CATCH()
}

void ICommImpl::joinGroupMembers(const std::string &group_name, const int64_t id, const int64_t size) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "Comm: member " << id << " of " << size << " preparing to join group " << group_name;
        MPI::Intracomm group = MPI::COMM_SELF;
        MPI::Intercomm client_comm;
        MPI::Intercomm info_comm;
        bool flag = true;
        if (id == 0) {
            MPI::Intercomm client_comms[size];
            auto handle = std::shared_ptr<MPI::Intercomm>(client_comms, [&size](MPI::Intercomm *client_comms) {
                for (int64_t i = 1; i < size; i++) {
                    if (client_comms[i] != MPI::COMM_NULL) {
                        client_comms[i].Free();
                    }
                }
            });

            for (int64_t i = 1; i < size; i++) {
                client_comm = MPI::COMM_SELF.Accept(group_name.c_str(), MPI::INFO_NULL, 0);
                int64_t pos;
                client_comm.Recv(&pos, 1, MPI::LONG_LONG_INT, 0, 1963);
                IGNIS_LOG(info) << "Comm: member " << id << " found";
                client_comms[pos] = client_comm;
            }
            IGNIS_LOG(info) << "Comm: all members found";

            for (int64_t i = 1; i < size; i++) {
                client_comm = client_comms[i];
                client_comm.Send(&flag, 1, MPI::BOOL, 0, 1963);
                info_comm = group.Accept(group_name.c_str(), MPI::INFO_NULL, 0);
                group = addComm(group, client_comm, MPI::COMM_SELF, true);
                IGNIS_LOG(info) << "Comm: new member added to the group";
                client_comm.Free();
                info_comm.Free();
            }
            IGNIS_LOG(info) << "Comm: all members added to the group";

        } else {
            group = MPI::COMM_NULL;
            IGNIS_LOG(info) << "Comm: connecting to the group " << group_name;
            client_comm = MPI::COMM_SELF.Connect(group_name.c_str(), MPI::INFO_NULL, 0);
            IGNIS_LOG(info) << "Comm: connected to the group, waiting for my turn";
            client_comm.Send(&id, 1, MPI::LONG_LONG_INT, 0, 1963);
            client_comm.Recv(&flag, 1, MPI::BOOL, 0, 1963);
            info_comm = MPI::COMM_SELF.Connect(group_name.c_str(), MPI::INFO_NULL, 0);
            group = addComm(group, client_comm, MPI::COMM_SELF, false);
            IGNIS_LOG(info) << "Comm: joined to the group";
            client_comm.Free();
            info_comm.Free();
            client_comm = MPI::COMM_NULL;

            for (int64_t i = id + 1; i < size; i++) {
                group.Accept(group_name.c_str(), MPI::INFO_NULL, 0).Free();
                IGNIS_LOG(info) << "Comm: new member added to the group";
                group = addComm(group, client_comm, MPI::COMM_SELF, true);
            }
        }
        const_cast<MPI::Intracomm &>(executor_data->getContext().mpiGroup()) = group;
        IGNIS_LOG(info) << "Comm: group ready with " << group.Get_size() << " members";
    IGNIS_CATCH()
}

void ICommImpl::joinToGroup(const std::string &group_name, const std::string &id) {
    IGNIS_TRY()
        MPI::Intracomm group = executor_data->getContext().mpiGroup();
        MPI::Intracomm new_group = group;
        MPI::Intercomm client_comm;
        bool member = executor_data->hasVariable("server");
        group.Bcast(&member, 1, MPI::BOOL, 0);
        if (executor_data->getContext().executorId() == 0) {
            MPI::Intercomm info_comm;
            if (member) {
                info_comm = group.Accept(group_name.c_str(), MPI::INFO_NULL, 0);
                client_comm = MPI::COMM_SELF.Accept(group_name.c_str(), MPI::INFO_NULL, 0);
                new_group = addComm(group, client_comm, MPI::COMM_SELF, false);
                client_comm.Free();
                info_comm.Free();
            } else {
                new_group = MPI::COMM_NULL;
                info_comm = group.Accept(group_name.c_str(), MPI::INFO_NULL, 0);
                client_comm = MPI::COMM_SELF.Accept(group_name.c_str(), MPI::INFO_NULL, 0);
                group = addComm(group, client_comm, MPI::COMM_SELF, false);
                client_comm.Free();
                info_comm.Free();
            }
        } else {
            client_comm = MPI::COMM_NULL;
            if (member) {
                group.Accept(group_name.c_str(), MPI::INFO_NULL, 0).Free();
                new_group = addComm(new_group, client_comm, MPI::COMM_SELF, false);
            } else {
                new_group = MPI::COMM_NULL;
                group.Accept(group_name.c_str(), MPI::INFO_NULL, 0).Free();
                new_group = addComm(new_group, client_comm, MPI::COMM_SELF, false);
            }
        }
        groups[id] = new_group;
    IGNIS_CATCH()
}

bool ICommImpl::hasGroup(const std::string &id) {
    IGNIS_TRY()
        return groups.find(id) != groups.end();
    IGNIS_CATCH()
}

void ICommImpl::destroyGroup(const std::string &id) {
    IGNIS_TRY()
        auto entry = groups.find(id);
        if (entry != groups.end()) {
            entry->second.Free();
            groups.erase(entry);
        }
    IGNIS_CATCH()
}

void ICommImpl::destroyGroups() {
    IGNIS_TRY()
        for (auto &elem: groups) {
            elem.second.Free();
        }
        groups.clear();
    IGNIS_CATCH()
}

void ICommImpl::setPartitionsVoid(const std::vector<std::string> &partitions) {
    IGNIS_TRY()
        auto group = executor_data->getPartitionTools().newPartitionGroup<storage::IVoidPartition::VOID_TYPE>();
        for (int64_t i = 0; i < partitions.size(); i++) {
            auto &bytes = partitions[i];
            auto buffer = std::make_shared<transport::IMemoryBuffer>((uint8_t *) bytes.c_str(), bytes.size());
            auto part = executor_data->getPartitionTools().newVoidPartition(bytes.size());
            part->read((std::shared_ptr<transport::ITransport> &) buffer);
            group->add(part);
        }
    IGNIS_CATCH()
}

void ICommImpl::driverScatterVoid(const std::string &id, const int64_t dataId) {
    IGNIS_TRY()
        //TODO
    IGNIS_CATCH()
}

MPI::Intracomm
ICommImpl::addComm(MPI::Intracomm &group, MPI::Intercomm &comm, MPI::Intracomm &local, bool detroyGroup) {
    MPI::Intercomm peer;
    MPI::Intercomm new_comm;
    MPI::Intracomm new_group;
    bool up = group == MPI::COMM_NULL;

    if (comm != MPI::COMM_NULL) {
        peer = comm.Merge(up);
    }

    if (up) {//new member
        new_comm = local.Create_intercomm(0, peer, 0, 1963);
    } else {
        new_comm = group.Create_intercomm(0, peer, 1, 1963);
    }

    new_group = new_comm.Merge(up);

    if (peer != MPI::COMM_NULL) {
        peer.Free();
    }
    new_comm.Free();

    if (detroyGroup) {
        group.Free();
    }

    return new_group;
}







