#include "Common.hpp"
#include "Lock.hpp"
#include "Block.hpp"
#include "Database.hpp"

int main(int argc, char* argv[]) {
    AK::Database db("./DB.bin", "AK_DB");
    return db.run(argc, argv);
}