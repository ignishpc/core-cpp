
#ifndef IGNIS_IEXCEPTION_H
#define IGNIS_IEXCEPTION_H

#include "ignis/executor/core/RTTInfo.h"
#include <memory>
#include <mpi.h>
#include <stdexcept>

namespace ignis {
    namespace executor {
        namespace core {
            namespace exception {

                class IException : public std::runtime_error {

                public:
                    IException(const IException &other);

                    IException(const std::string &message);

                    IException(const std::string &message, const IException &cause);

                    IException(const std::exception &cause);

                    virtual ~IException();

                    virtual bool hasCause() const;

                    virtual const IException &getCause() const;

                    virtual void printStackTrace(std::ostream &stream) const;

                    virtual std::string getStacktrace() const;

                    virtual void print(std::ostream &stream) const;

                    virtual std::string toString() const;

                    virtual std::ostream &operator<<(std::ostream &stream) const;

                protected:
                    virtual std::string exceptionName() const;

                    template<typename Ex>
                    std::string parseName() const {
                        return RTTInfo::from<Ex>().getStandardName();
                    }

                private:
                    const std::shared_ptr<IException> cause;
                    const std::shared_ptr<void> stacktrace;
                };

                inline std::ostream &operator<<(std::ostream &stream, const IException &ex) {
                    ex.operator<<(stream);
                    return stream;
                }
            }// namespace exception
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#define IGNIS_TRY() try {

#define IGNIS_CATCH()                                                                                                  \
    }                                                                                                                  \
    catch (ignis::executor::core::exception::IException & ex) {                                                        \
        throw ex;                                                                                                      \
    }                                                                                                                  \
    catch (std::exception & ex) {                                                                                      \
        throw ignis::executor::core::exception::IException(ex);                                                        \
    }                                                                                                                  \
    catch (MPI::Exception & ex) {                                                                                      \
        throw ignis::executor::core::exception::IException(std::string("MPI::Exception ") + ex.Get_error_string());                 \
    }                                                                                                                  \
    catch (...) {                                                                                                      \
        throw ignis::executor::core::exception::IException("unknown exception");                                       \
    }

#define IGNIS_RPC_TRY() try {

#define IGNIS_RPC_CATCH()                                                                                              \
    }                                                                                                                  \
    catch (ignis::executor::core::exception::IException & ex) {                                                        \
        ignis::rpc::IExecutorException eex;                                                                            \
        eex.__set_message(ex.what());                                                                                  \
        eex.__set_cause_(ex.toString());                                                                               \
        IGNIS_LOG(error) << ex.toString();                                                                             \
        throw eex;                                                                                                     \
    }                                                                                                                  \
    catch (std::exception & ex) {                                                                                      \
        ignis::rpc::IExecutorException eex;                                                                            \
        eex.__set_message(ex.what());                                                                                  \
        eex.__set_cause_("");                                                                                          \
        IGNIS_LOG(error) << ex.what();                                                                                 \
        throw eex;                                                                                                     \
    }                                                                                                                  \
    catch (MPI::Exception & ex) {                                                                                      \
        ignis::rpc::IExecutorException eex;                                                                            \
        eex.__set_message(std::string("MPI::Exception ") + ex.Get_error_string());                                                  \
        eex.__set_cause_("");                                                                                          \
        IGNIS_LOG(error) << "MPI::Exception " << ex.Get_error_string();                                                 \
        throw eex;                                                                                                     \
    }                                                                                                                  \
    catch (...) {                                                                                                      \
        ignis::rpc::IExecutorException eex;                                                                            \
        eex.__set_message("unknown exception");                                                                        \
        eex.__set_cause_("");                                                                                          \
        IGNIS_LOG(error) << "unknown exception";                                                                       \
        throw eex;                                                                                                     \
    }

#define IGNIS_OMP_EXCEPTION_INIT()                                                                                     \
    std::shared_ptr<ignis::executor::core::exception::IException> ignis_parallel_exception;

#define IGNIS_OMP_TRY() try {

#define IGNIS_OMP_CATCH()                                                                                              \
    }                                                                                                                  \
    catch (ignis::executor::core::exception::IException & ex) {                                                        \
        ignis_parallel_exception = std::make_shared<ignis::executor::core::exception::IException>(ex);                 \
    }                                                                                                                  \
    catch (std::exception & ex) {                                                                                      \
        ignis_parallel_exception = std::make_shared<ignis::executor::core::exception::IException>(ex);                 \
    }                                                                                                                  \
    catch (MPI::Exception & ex) {                                                                                      \
        ignis_parallel_exception = std::make_shared<ignis::executor::core::exception::IException>(                     \
                std::string("MPI::Exception ") + ex.Get_error_string());                                                            \
    }                                                                                                                  \
    catch (...) {                                                                                                      \
        ignis_parallel_exception =                                                                                     \
                std::make_shared<ignis::executor::core::exception::IException>("unknown exception");                   \
    }


#define IGNIS_OMP_EXCEPTION_END()                                                                                      \
    if (ignis_parallel_exception) { throw ignis::executor::core::exception::IException(*ignis_parallel_exception); }

#endif
