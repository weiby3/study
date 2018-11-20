#include <boost/mpi.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace boost;

int main(int argc, char *argv[]) {
    mpi::environment env(argc, argv);
    mpi::communicator world;
    // 1
    //cout << world.rank() << ", " << world.size() << endl;

    // 5
    string s;
    if (world.rank() == 0) {
        s = "Hello, World!";
    }
    mpi::broadcast(world, s, 0);
    cout << s << endl;
    if (world.rank() == 0) {
        // 2
        //int i;
        //world.recv(mpi::any_source, 16, i);
        //cout << i << endl;

        // 3
        //string s;
        //mpi::request r = world.irecv(mpi::any_source, 16, s);
        //if (r.test()) {
        //    cout << s << endl;
        //}

        //4
        //mpi::request requests[2];
        //string s[2];
        //requests[0] = world.irecv(1, 16, s[0]);
        //requests[1] = world.irecv(2, 16, s[1]);
        //mpi::wait_all(requests, requests + 2);
        //cout << s[0] << "; " << s[1] << endl;
    } else if (world.rank() == 1) {
        // 2
        //world.send(0, 16, 99);

        // 3
        //string s("Hello, World!");
        //world.send(0, 16, s);

        //4
        //string s("Hello, World!");
        //world.send(0, 16, s);
    } else if (world.rank() == 2) {
        //4
        //string s("Hello, World!");
        //world.send(0, 16, s);
    }
}