#define __PROGTEST__
#include <vector>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <iostream>
using namespace std;
#include "test.cpp"


bool writeTest(CFile & x, std::fstream & fout,
               const char * data,
               uint32_t wrLen) {
    fout.write(data, wrLen);
    assert(fout);
    return x.write(reinterpret_cast<const uint8_t *>(data), wrLen) == wrLen;
}

bool readTest(CFile & x,
              std::fstream & fin,
              uint32_t rdLen) {
    uint8_t tmp[1000];
    char buffer[1000];
    fin.read(buffer, 1000);
    long real_count = fin.gcount();
    uint32_t idx = 0;
    unsigned int bytes = x.read(tmp, rdLen);
    if (bytes != real_count) {
        std::cout << "Error with read length yours: " << bytes << " correct: " << real_count << std::endl;
        return false;
    }
    for (int i = 0; i < real_count; ++i)
        if (tmp[idx++] != buffer[i]) {
            std::cout << "Miss match:\nCorrect: " << buffer << "\nYours:  " << tmp << std::endl;
            return false;
        }
    return true;
}

#include <filesystem>

int version_number = 0;
std::vector<int> positions;
std::ostringstream current_version;

int random_seek(CFile & cfile, std::fstream & file) {
    if (cfile.fileSize() > 0) {
        cfile.seek(cfile.fileSize());
        int position = rand() % (cfile.fileSize());
        cfile.seek(position);
        file.seekg(position, std::ios::beg);

        if (rand() > RAND_MAX / 3) {
            cfile.addVersion();
            positions.push_back(position);
            std::ostringstream new_version;
            ++version_number;
            new_version << "test_file.txt" << std::setfill('0') << std::setw(5) << (version_number);
            std::filesystem::copy(current_version.str(), new_version.str());
            current_version.str(new_version.str());
        } else {
            if(cfile.undoVersion()){
                std::ostringstream new_version;
                --version_number;
                new_version << "test_file.txt" << std::setfill('0') << std::setw(5) << (version_number);
                std::filesystem::remove(current_version.str());
                current_version.str(new_version.str());
                position = positions.back();
                positions.pop_back();
            }
        }

        return position;
    }
    return -1;
}


void super_dooper_tester() {

    CFile cfile;
    bool only_clean = false;

    clean_up:
    int current_delete = 0;
    std::ostringstream new_version;
    new_version << "test_file.txt" << std::setfill('0') << std::setw(5) << (current_delete);
    while (std::filesystem::exists(new_version.str())) {
        std::filesystem::remove(new_version.str());
        ++current_delete;
        new_version.str("");
        new_version << "test_file.txt" << std::setfill('0') << std::setw(5) << (current_delete);
    }
    if (only_clean) {
        exit(0);
    }

    current_version << "test_file.txt" << std::setfill('0') << std::setw(5) << version_number;
    std::ofstream out;
    out.open(current_version.str());
    out.close();

    std::fstream fout(current_version.str(), std::ios::binary | std::ios::out | std::ios::in);

    // netestuje se na edge case vytvoreni verze s prazdnym souborem
    int count = 200; //200-1000
    for (int i = 0; i < count; ++i) {
        if (i % (int)(count / 100.0) == 0) {
            std::cout << "Test " << i / (count / 100) << "% done cfile size:" << cfile.fileSize() << " Number of versions:"
                      << version_number << std::endl;
            if (i % (count / 200) == 0) {
                while (cfile.undoVersion()){
                    std::ostringstream new_version;
                    --version_number;
                    new_version << "test_file.txt" << std::setfill('0') << std::setw(5) << (version_number);
                    std::filesystem::remove(current_version.str());
                    current_version.str(new_version.str());
                    fout.seekg(positions.back(), std::ios::beg);
                    positions.pop_back();
                }
            }
        }
        for (int k = 0; k < 10000; ++k) {
            // READ seek and truncate testing
            if (cfile.fileSize() > 0) {

                int position = random_seek(cfile, fout);

                fout << std::flush;
                fout.close();

                fout.seekg(position, std::ios::beg);

                cfile.truncate();
                assert(readTest(cfile, fout, 999));
                std::filesystem::resize_file(current_version.str(), position);
                fout.open(current_version.str());
                fout.seekg(position, std::ios::beg);
                cfile.truncate();
            }
        }

        for (int p = 0; p < 10000; ++p) {
            random_seek(cfile, fout);
        }
        // Copy test and = test
        std::vector<CFile> files;
        for (int l = 0; l < 3; ++l) {
            files.push_back(cfile);
        }
        cfile = files[rand() % files.size()];
        files.clear();

        // Testing writes
        std::ostringstream oss;
        oss.str("");
        for (int p = 0; p < 20; ++p) {
            oss.clear();
            oss.str("");
            for (int j = 0; j < 10; ++j)
                oss << (char) ('a' + rand() % 26);
            oss << " \n";
            if (rand() > RAND_MAX / 2) {
                random_seek(cfile, fout);
            }
            assert(writeTest(cfile, fout, oss.str().c_str(), oss.str().length()));
        }
    }
    only_clean = true;
    goto clean_up;
}

int main(void){
    super_dooper_tester();
}
