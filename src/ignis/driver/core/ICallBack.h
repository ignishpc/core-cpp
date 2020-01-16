

#ifndef IGNIS_ICALLBACK_H
#define IGNIS_ICALLBACK_H

namespace ignis {
    namespace driver {
        namespace core {
            class ICallBack {
            public:
                ICallBack(int port, int compression);

                virtual ~ICallBack();
            };
        }
    }
}

#endif
