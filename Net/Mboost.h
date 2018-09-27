//
// Created by wj on 18-9-26.
//

#ifndef NET_BOOST_H
#define NET_BOOST_H
namespace Mboost {
        class noncopyable {
        protected:
            noncopyable()
            {

            }
            ~noncopyable()
            {

            }

        private:
            noncopyable(noncopyable &on)
            {

            }
            noncopyable & operator=(noncopyable &on)
            {

            }

        };

}
#endif //NET_BOOST_H
