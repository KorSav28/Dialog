#include "dialog.h"
#include "iostream"
#include "string.h"

Chat::Chat() {
    data_count = 0;
    data = nullptr; 
    allocNewMem(8);
}

void Chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    uint* digest = sha1(_pass, pass_length); 
    add_in_table(_login, digest);
}

void Chat::add_in_table(char login[LOGINLENGTH], uint* digest) {
    int index = -1, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(login, i*i); 
        if (data[index].status == enPairStatus::free) {
            break;
        }
    }
    if (i >= mem_size)
    {
        resize();
        add_in_table(login, digest);
    }
    else {
        data[index] = AuthData(login, digest);
        data_count++;
    }
}

void Chat::del(char _login[LOGINLENGTH]){
    int index = -1, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(_login, i*i); 
        if (data[index].status == enPairStatus::engaged &&
            !memcmp(_login, data[index].login, LOGINLENGTH)) {
            data[index].status = enPairStatus::deleted;
            data_count--;
            return;
        }
        else if (data[index].status == enPairStatus::free) {
            return;
        }
    }
    if (i >= mem_size) return; 
}
bool Chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    
    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(_login, i*i); 
        if (data[index].status == enPairStatus::free)
            return false;
        else if (data[index].status == enPairStatus::engaged
            && !memcmp(_login, data[index].login, LOGINLENGTH))
            break;
    }
    if (i >= mem_size) return false;

    uint* digest = sha1(_pass, pass_length);

    bool cmpHashes = !memcmp( 
        data[index].pass_sha1_hash, 
        digest,
        SHA1HASHLENGTHBYTES); 
    delete[] digest; 

    return cmpHashes; 
}

void Chat::allocNewMem(int newMemSize) {
    mem_size = newMemSize;
    data = new AuthData[mem_size];
}

int Chat::hash_func(char login[LOGINLENGTH], int step) {
    long sum = 0;
    for (int i = 0; i < LOGINLENGTH; i++) {
        sum += login[i];
    }
    return (hf_umn(sum) + step) % mem_size; 
}

int Chat::hf_umn(int value) 
{
    const double A = 0.6;
    return int(mem_size * (A * value - int(A * value)));
}

void Chat::resize() {

    AuthData* save = data; 
    int save_ms = mem_size;

    mem_size *= 2;
    data = new AuthData[mem_size];
    data_count = 0;

    for (int i = 0; i < save_ms; i++) {
        AuthData& old_pair = save[i];
        if (old_pair.status == enPairStatus::engaged) {
            uint* sha_hash_copy = new uint[SHA1HASHLENGTHUINTS];
            memcpy(sha_hash_copy, old_pair.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            add_in_table(old_pair.login, sha_hash_copy); 
        }
    }

    delete[] save;
}