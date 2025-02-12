#pragma once

#include "Sha1.h"
#include "string.h"

#define SIZE 10
#define LOGINLENGTH 10

class Chat {
public:
    Chat();

    void reg(char _login[LOGINLENGTH], char _pass[], int pass_length);
    bool login(char _login[LOGINLENGTH], char _pass[], int pass_length);
    void del(char _login[LOGINLENGTH]);

private:
    enum enPairStatus {
        free,
        engaged,
        deleted
    };

    struct AuthData {
        AuthData() :
            login(""),
            pass_sha1_hash(0),
            status (enPairStatus::free){ 
        }
        ~AuthData() {
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
        }
        AuthData(char _login[LOGINLENGTH], uint* sh1) {
            memcpy(login, _login, LOGINLENGTH);
            pass_sha1_hash = sh1;
            status = enPairStatus::engaged;
        }
        AuthData& operator = (const AuthData& other) {
            memcpy (login, other.login, LOGINLENGTH); 

            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);
            status = other.status;
            return *this;
        }
        char login[LOGINLENGTH];
        uint* pass_sha1_hash;
        enPairStatus status; 

    };
    void resize();
    void allocNewMem(int newMemSize); 
    int hash_func(char login[LOGINLENGTH], int step); 
    int hf_umn(int value);
    void add_in_table(char login[LOGINLENGTH], uint* digest);

    AuthData* data;
    int data_count;
    int mem_size;
};