
#include "IIOModuleTest.h"
#include <fstream>
#include <ignis/executor/api/IJsonValue.h>

using namespace ignis::executor::core::modules;

IIOModuleTest::IIOModuleTest() : io(std::make_shared<IIOModule>(executor_data)) {
    auto &props = executor_data->getContext().props();
    props["ignis.partition.minimal"] = "10MB";
    props["ignis.partition.type"] = "Memory";
    props["ignis.modules.io.overwrite"] = "true";
    props["ignis.modules.io.cores"] = "1";
    props["ignis.modules.io.compression"] = "0";
}

void IIOModuleTest::setUp() {}

void IIOModuleTest::tearDown() {}

void IIOModuleTest::voidWithCompileTest(bool compileTest) {
    auto elems = IElements<std::string>().create(100, 0);
    auto buffer = std::make_shared<transport::IMemoryBuffer>();
    auto trans = std::make_shared<transport::IZlibTransport>(buffer, 9);
    protocol::IObjectProtocol(trans).writeObject(elems);
    trans->flush();
    auto vpart = executor_data->getPartitionTools().newVoidPartition();
    auto vgroup = executor_data->getPartitionTools().newPartitionGroup<storage::IVoidPartition::VOID_TYPE>();
    vpart->read(reinterpret_cast<std::shared_ptr<transport::ITransport> &>(buffer));
    vgroup->add(vpart);
    executor_data->setPartitions(vgroup);

    if (compileTest) {
        auto src = ghc::filesystem::current_path();
        while (src.root_directory() != src && !ghc::filesystem::exists(src / "src/ignis")) { src = src.parent_path(); }

        if (!ghc::filesystem::exists(src / "src/ignis")) {
            std::cerr << " [Partial] ";
        } else {
            std::string folder = executor_data->getProperties().executorDirectory() + "/cpptypes";
            if (ghc::filesystem::exists(folder)) { ghc::filesystem::remove_all(folder); }
            auto include = ghc::filesystem::path("core/cpp");
            ghc::filesystem::remove_all("core");
            ghc::filesystem::create_directories(include);
            ghc::filesystem::create_directory_symlink(src / "src", include / "include");
            setenv("IGNIS_HOME", "./", 1);
            CPPUNIT_ASSERT(io->typeFromPartition()->info() == RTTInfo::from<std::string>());
        }
    }
    auto result = getFromPartitions<std::string>();
    CPPUNIT_ASSERT(elems == result);
}

void IIOModuleTest::textFileTest(int n, int cores) {
    executor_data->setCores(cores);
    srand(0);
    const char alphanum[] = "0123456789"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";

    std::string path = "./tmpfile.txt";
    auto executors = executor_data->getContext().executors();
    std::ofstream file(path, std::fstream::trunc);
    std::vector<std::string> lines;
    std::string line;

    for (int l = 0; l < 10000; l++) {
        int lc = rand() % 100;
        for (int i = 0; i < lc; ++i) { line += alphanum[rand() % (sizeof(alphanum) - 1)]; }
        file << line << std::endl;
        lines.push_back(std::move(line));
    }

    io->textFile2(path, n);

    CPPUNIT_ASSERT_GREATEREQUAL(n / executors, (int) executor_data->getPartitions<std::string>()->partitions());

    auto result = getFromPartitions<std::string>();

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::string>())[0]), 0);

    result = getFromPartitions<std::string>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(lines.size(), result.size());
        for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(lines[i], result[i]); }
    }
}

void IIOModuleTest::plainFileTest(int n, int cores, const std::string &delim, const std::string& ex) {
    executor_data->setCores(cores);
    srand(0);
    const char alphanum[] = "0123456789"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";

    std::string path = "./plainfile.txt";
    auto executors = executor_data->getContext().executors();
    std::ofstream file(path, std::fstream::trunc);
    std::vector<std::string> lines;
    std::string line;
    std::string delim2 = delim;

    for (int l = 0; l < 10000; l++) {
        int lc = rand() % 100;
        for (int i = 0; i < lc; ++i) { line += alphanum[rand() % (sizeof(alphanum) - 1)]; }
        if(!ex.empty()){
            line += ex + delim;
        }
        file << line << delim;
        lines.push_back(std::move(line));
    }
    file.flush();

    auto replaceAll = [](std::string &subject, const std::string &search, const std::string &replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    };

    if(!ex.empty()){
        std::string ex2 = ex;
        replaceAll(ex2,"!", "\\!");
        delim2 += "!" + ex2;
    }

    io->plainFile3(path, n, delim2);

    CPPUNIT_ASSERT_GREATEREQUAL(n / executors, (int) executor_data->getPartitions<std::string>()->partitions());

    auto result = getFromPartitions<std::string>();

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::string>())[0]), 0);

    result = getFromPartitions<std::string>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(lines.size(), result.size());
        for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(lines[i], result[i]); }
    }
}

void IIOModuleTest::saveAsTextFileTest(int n, int cores) {
    executor_data->setCores(cores);
    srand(0);
    const char alphanum[] = "0123456789"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";

    auto id = executor_data->getContext().executorId();
    registerType<std::string>();
    api::IVector<std::string> lines;
    std::string line;

    for (int l = 0; l < 10000; l++) {
        int lc = rand() % 100;
        for (int i = 0; i < lc; ++i) { line += alphanum[rand() % (sizeof(alphanum) - 1)]; }
        lines.push_back(std::move(line));
    }

    loadToPartitions(lines, n);

    io->saveAsTextFile("./tmpsave", cores * id);
}


void IIOModuleTest::partitionTextFileTest() {
    std::string path = "./savetest.txt";
    int n = 8;
    auto rank = executor_data->mpi().rank();
    srand(0);
    const char alphanum[] = "0123456789"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";
    registerType<std::string>();
    api::IVector<std::string> lines;
    std::string line;

    for (int l = 0; l < 10000; l++) {
        int lc = rand() % 100;
        for (int i = 0; i < lc; ++i) { line += alphanum[rand() % (sizeof(alphanum) - 1)]; }
        lines.push_back(std::move(line));
    }

    loadToPartitions(lines, n);
    if (ghc::filesystem::exists(path)) { ghc::filesystem::remove_all(path); }
    io->saveAsTextFile(path, rank * n);
    io->partitionTextFile(path, rank * n, n);

    auto result = getFromPartitions<std::string>();
    CPPUNIT_ASSERT(lines == result);
}

void IIOModuleTest::partitionJsonFileTestImpl(bool objMap) {
    std::string path = "./savetest.txt";
    int n = 8;
    auto rank = executor_data->mpi().rank();
    srand(0);
    const char alphanum[] = "0123456789"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";
    registerType<std::string>();
    api::IVector<std::string> lines;
    std::string line;

    for (int l = 0; l < 10000; l++) {
        int lc = rand() % 100;
        for (int i = 0; i < lc; ++i) { line += alphanum[rand() % (sizeof(alphanum) - 1)]; }
        lines.push_back(std::move(line));
    }

    loadToPartitions(lines, n);
    if (ghc::filesystem::exists(path)) { ghc::filesystem::remove_all(path); }
    io->saveAsJsonFile(path, rank * n, true);
    if (objMap) {
        io->partitionJsonFile4b(path, rank * n, n, newSource("TypeString"));
        auto result = getFromPartitions<std::string>();
        CPPUNIT_ASSERT(lines == result);
    } else {
        io->partitionJsonFile4a(path, rank * n, n, false);
        auto result = getFromPartitions<api::IJsonValue>();
        CPPUNIT_ASSERT_EQUAL(lines.size(), result.size());
        for (int64_t i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(lines[i], result[i].getString()); }
    }
}

void IIOModuleTest::partitionObjectFileTest() {
    std::string path = "./savetest.txt";
    int n = 8;
    auto rank = executor_data->mpi().rank();
    srand(0);
    const char alphanum[] = "0123456789"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";
    registerType<std::string>();
    api::IVector<std::string> lines;
    std::string line;

    for (int l = 0; l < 10000; l++) {
        int lc = rand() % 100;
        for (int i = 0; i < lc; ++i) { line += alphanum[rand() % (sizeof(alphanum) - 1)]; }
        lines.push_back(std::move(line));
    }

    loadToPartitions(lines, n);
    if (ghc::filesystem::exists(path)) { ghc::filesystem::remove_all(path); }
    io->saveAsObjectFile(path, 0, rank * n);
    io->partitionObjectFile(path, rank * n, n);

    auto result = getFromPartitions<std::string>();
    CPPUNIT_ASSERT(lines == result);
}