#include "tiny_string.hxx"

namespace tiny_string {
    void fastring::ensure_capacity(size_t new_size) {
        if (new_size > capacity_) {
            size_t new_capacity = std::max(new_size, capacity_ * 2);
            char* new_data = new char[new_capacity + 1];
            std::memcpy(new_data, data_, size_);
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    constexpr fastring::fastring() noexcept : data_(nullptr), size_(0), capacity_(0) {}

    fastring::fastring(size_t cap) : data_(new char[cap + 1]), size_(0), capacity_(cap) {
        data_[0] = '\0';
    }

    fastring::fastring(const void* s, size_t n) : data_(new char[n + 1]), size_(n), capacity_(n) {
        std::memcpy(data_, s, n);
        data_[n] = '\0';
    }

    fastring::fastring(const char* s) : data_(nullptr), size_(0), capacity_(0) {
        if (s) {
            size_ = std::strlen(s);
            capacity_ = size_;
            data_ = new char[capacity_ + 1];
            std::strcpy(data_, s);
        }
    }

    fastring::fastring(const std::string& s) : data_(new char[s.size() + 1]), size_(s.size()), capacity_(s.size()) {
        std::strcpy(data_, s.c_str());
    }

    fastring::fastring(size_t n, char c) : data_(new char[n + 1]), size_(n), capacity_(n) {
        std::fill(data_, data_ + n, c);
        data_[n] = '\0';
    }

    fastring::fastring(const fastring& s) : data_(new char[s.size_ + 1]), size_(s.size_), capacity_(s.size_) {
        std::strcpy(data_, s.data_);
    }

    fastring::fastring(fastring&& s) noexcept : data_(s.data_), size_(s.size_), capacity_(s.capacity_) {
        s.data_ = nullptr;
        s.size_ = 0;
        s.capacity_ = 0;
    }

    fastring::~fastring() {
        delete[] data_;
    }

    fastring& fastring::operator=(const char* s) {
        if (s) {
            size_t new_size = std::strlen(s);
            ensure_capacity(new_size);
            size_ = new_size;
            std::strcpy(data_, s);
        } else {
            size_ = 0;
            if (data_) {
                data_[0] = '\0';
            }
        }
        return *this;
    }

    fastring& fastring::operator=(const std::string& s) {
        size_t new_size = s.size();
        ensure_capacity(new_size);
        size_ = new_size;
        std::strcpy(data_, s.c_str());
        return *this;
    }

    fastring& fastring::operator=(const fastring& s) {
        if (this != &s) {
            delete[] data_;
            size_ = s.size_;
            capacity_ = s.size_;
            data_ = new char[capacity_ + 1];
            std::strcpy(data_, s.data_);
        }
        return *this;
    }

    fastring& fastring::operator=(fastring&& s) noexcept {
        if (this != &s) {
            delete[] data_;
            data_ = s.data_;
            size_ = s.size_;
            capacity_ = s.capacity_;
            s.data_ = nullptr;
            s.size_ = 0;
            s.capacity_ = 0;
        }
        return *this;
    }

    fastring& fastring::assign(const void* s, size_t n) {
        ensure_capacity(n);
        size_ = n;
        std::memcpy(data_, s, n);
        data_[n] = '\0';
        return *this;
    }

    template<typename S>
    fastring& fastring::assign(S&& s) {
        return (*this = std::forward<S>(s));
    }

    fastring& fastring::assign(size_t n, char c) {
        ensure_capacity(n);
        size_ = n;
        std::fill(data_, data_ + n, c);
        data_[n] = '\0';
        return *this;
    }

    char& fastring::back() {
        return data_[size_ - 1];
    }

    const char& fastring::back() const {
        return data_[size_ - 1];
    }

    char& fastring::front() {
        return data_[0];
    }

    const char& fastring::front() const {
        return data_[0];
    }

    char& fastring::operator[](size_t n) {
        return data_[n];
    }

    const char& fastring::operator[](size_t n) const {
        return data_[n];
    }

    size_t fastring::capacity() const noexcept {
        return capacity_;
    }

    const char* fastring::c_str() const {
        return data_;
    }

    char* fastring::data() noexcept {
        return data_;
    }

    const char* fastring::data() const noexcept {
        return data_;
    }

    bool fastring::empty() const noexcept {
        return size_ == 0;
    }

    size_t fastring::size() const noexcept {
        return size_;
    }

    void fastring::clear() {
        size_ = 0;
        if (data_) {
            data_[0] = '\0';
        }
    }

    void fastring::clear(char c) {
        if (data_) {
            std::fill(data_, data_ + capacity_, c);
        }
        size_ = 0;
        if (data_) {
            data_[0] = '\0';
        }
    }

    void fastring::ensure(size_t n) {
        ensure_capacity(size_ + n);
    }

    void fastring::reserve(size_t n) {
        if (n > capacity_) {
            char* new_data = new char[n + 1];
            std::memcpy(new_data, data_, size_);
            delete[] data_;
            data_ = new_data;
            capacity_ = n;
        }
    }

    void fastring::reset() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    void fastring::resize(size_t n) {
        if (n > size_) {
            ensure_capacity(n);
        }
        size_ = n;
        data_[size_] = '\0';
    }

    void fastring::resize(size_t n, char c) {
        if (n > size_) {
            ensure_capacity(n);
            std::fill(data_ + size_, data_ + n, c);
        }
        size_ = n;
        data_[size_] = '\0';
    }

    void fastring::shrink() {
        if (size_ < capacity_) {
            char* new_data = new char[size_ + 1];
            std::memcpy(new_data, data_, size_);
            delete[] data_;
            data_ = new_data;
            capacity_ = size_;
        }
    }

    void fastring::swap(fastring& s) noexcept {
        std::swap(data_, s.data_);
        std::swap(size_, s.size_);
        std::swap(capacity_, s.capacity_);
    }

    void fastring::swap(fastring&& s) noexcept {
        swap(s);
    }

    fastring& fastring::append(const void* s, size_t n) {
        ensure(size_ + n);
        std::memcpy(data_ + size_, s, n);
        size_ += n;
        data_[size_] = '\0';
        return *this;
    }

    fastring& fastring::append(const char* s) {
        if (s) {
            size_t len = std::strlen(s);
            ensure(size_ + len);
            std::strcpy(data_ + size_, s);
            size_ += len;
        }
        return *this;
    }

    fastring& fastring::append(const fastring& s) {
        return append(s.data_, s.size_);
    }

    fastring& fastring::append(const std::string& s) {
        return append(s.c_str(), s.size());
    }

    fastring& fastring::append(size_t n, char c) {
        ensure(size_ + n);
        std::fill(data_ + size_, data_ + size_ + n, c);
        size_ += n;
        data_[size_] = '\0';
        return *this;
    }

    fastring& fastring::append(char c) {
        ensure(size_ + 1);
        data_[size_++] = c;
        data_[size_] = '\0';
        return *this;
    }

    fastring& fastring::append_nomchk(const void* s, size_t n) {
        return append(s, n);
    }

    fastring& fastring::append_nomchk(const char* s) {
        return append(s);
    }

    template<typename X, typename ...V>
    fastring& fastring::cat(X&& x, V&& ... v) {
        (*this) << std::forward<X>(x);
        if constexpr (sizeof...(v) > 0) {
            return cat(std::forward<V>(v)...);
        }
        return *this;
    }

    fastring& fastring::operator<<(bool v) {
        return append(v ? "true" : "false");
    }

    fastring& fastring::operator<<(char v) {
        return append(v);
    }

    fastring& fastring::operator<<(signed char v) {
        return append(static_cast<char>(v));
    }

    fastring& fastring::operator<<(unsigned char v) {
        return append(static_cast<char>(v));
    }

    fastring& fastring::operator<<(short v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%hd", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(unsigned short v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%hu", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(int v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%d", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(unsigned int v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%u", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(long v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%ld", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(unsigned long v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%lu", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(long long v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%lld", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(unsigned long long v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%llu", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(double v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%f", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(float v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%f", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(const void* v) {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%p", v);
        return append(buffer);
    }

    fastring& fastring::operator<<(std::nullptr_t) {
        return append("nullptr");
    }

    fastring& fastring::operator<<(const char* s) {
        return append(s);
    }

    fastring& fastring::operator<<(const signed char* s) {
        return append(reinterpret_cast<const char*>(s));
    }

    fastring& fastring::operator<<(const unsigned char* s) {
        return append(reinterpret_cast<const char*>(s));
    }

    fastring& fastring::operator<<(const fastring& s) {
        return append(s);
    }

    fastring& fastring::operator<<(const std::string& s) {
        return append(s);
    }

    // += 运算符重载
    fastring& fastring::operator+=(const char* s) {
        return append(s);
    }

    fastring& fastring::operator+=(const fastring& s) {
        return append(s);
    }

    fastring& fastring::operator+=(const std::string& s) {
        return append(s);
    }

    fastring& fastring::operator+=(char c) {
        return append(c);
    }

    // push_back 方法
    fastring& fastring::push_back(char c) {
        return append(c);
    }

    // pop_back 方法
    char fastring::pop_back() {
        char last = data_[size_ - 1];
        data_[--size_] = '\0';
        return last;
    }

    // compare 方法
    int fastring::compare(const char* s, size_t n) const {
        return std::strncmp(data_, s, std::min(size_, n));
    }

    int fastring::compare(const char* s) const {
        return std::strcmp(data_, s);
    }

    int fastring::compare(const fastring& s) const noexcept {
        return std::strcmp(data_, s.data_);
    }

    int fastring::compare(const std::string& s) const noexcept {
        return std::strcmp(data_, s.c_str());
    }

    int fastring::compare(size_t pos, size_t len, const char* s, size_t n) const {
        if (pos >= size_) return -1;
        len = std::min(len, size_ - pos);
        return std::strncmp(data_ + pos, s, std::min(len, n));
    }

    int fastring::compare(size_t pos, size_t len, const char* s) const {
        if (pos >= size_) return -1;
        len = std::min(len, size_ - pos);
        return std::strncmp(data_ + pos, s, len);
    }

    int fastring::compare(size_t pos, size_t len, const fastring& s) const {
        return compare(pos, len, s.data_, s.size_);
    }

    int fastring::compare(size_t pos, size_t len, const std::string& s) const {
        return compare(pos, len, s.c_str(), s.size());
    }

    int fastring::compare(size_t pos, size_t len, const fastring& s, size_t spos, size_t n) const {
        if (pos >= size_ || spos >= s.size_) return -1;
        len = std::min(len, size_ - pos);
        n = std::min(n, s.size_ - spos);
        return std::strncmp(data_ + pos, s.data_ + spos, std::min(len, n));
    }

    int fastring::compare(size_t pos, size_t len, const std::string& s, size_t spos, size_t n) const {
        if (pos >= size_ || spos >= s.size()) return -1;
        len = std::min(len, size_ - pos);
        n = std::min(n, s.size() - spos);
        return std::strncmp(data_ + pos, s.c_str() + spos, std::min(len, n));
    }
}