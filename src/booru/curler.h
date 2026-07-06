#pragma once

#include <atomic>
#include <chrono>
#include <curl/curl.h>
#include <giomm.h>
#include <glibmm.h>
#include <mutex>

namespace AhoViewer::Booru
{
    using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;

    class ImageFetcher;
    class Curler
    {
        friend class ImageFetcher;

        using SignalWriteType = sigc::signal<void, const unsigned char*, size_t>;

    public:
        Curler(const std::string& url = "", CURLSH* share = nullptr);
        ~Curler();

        void set_url(std::string url);
        void set_follow_location(const bool n) const;
        void set_referer(const std::string& url) const;
        void set_user_agent(const std::string& ua) const;
        void set_http_auth(const std::string& u, const std::string& p) const;
        void set_share_handle(CURLSH* s) const;

        std::string escape(const std::string& str) const;
        bool perform();

        void set_imagefetcher(ImageFetcher* f) { m_ImageFetcher = f; }
        void clear()
        {
            // m_Buffer is written to by write_cb on the ImageFetcher/perform thread, so
            // guard it here to avoid freeing the buffer out from under a concurrent write
            std::scoped_lock lock{ m_BufferMutex };
            m_Buffer.clear();
            std::vector<unsigned char>().swap(m_Buffer);
        }
        void save_file(const std::string& path) const;
        void save_file_async(const std::string& path, const Gio::SlotAsyncReady& cb);
        void save_file_finish(const Glib::RefPtr<Gio::AsyncResult>& r);

        void get_progress(curl_off_t& current, curl_off_t& total);

        bool is_active() const { return m_Active; }
        std::string get_url() const { return m_Url; }

        const std::vector<unsigned char> get_buffer()
        {
            std::scoped_lock lock{ m_BufferMutex };
            return m_Buffer;
        }
        unsigned char* get_data() { return m_Buffer.data(); }
        size_t get_data_size() const { return m_Buffer.size(); }

        std::string get_error() const { return curl_easy_strerror(m_Response); }
        CURLcode get_response() const { return m_Response; }
        // HTTP reponse code
        long get_response_code() const;
        time_point_t get_start_time() const { return m_StartTime; }

        void pause();
        // These will either unpause immediatly, or queue the handle
        // to be do the unpase in the ImageFether thread.
        // Currently only used when it's the 2nd scenerio
        void unpause();

        void cancel() { m_Cancel->cancel(); }
        bool is_cancelled() const { return m_Cancel->is_cancelled(); }

        SignalWriteType signal_write() const { return m_SignalWrite; }
        Glib::Dispatcher& signal_progress() { return m_SignalProgress; }
        Glib::Dispatcher& signal_finished() { return m_SignalFinished; }

    private:
        static size_t write_cb(const unsigned char* ptr, size_t size, size_t nmemb, void* userp);
        static int progress_cb(void* userp, curl_off_t, curl_off_t, curl_off_t, curl_off_t);

        static const char* DefaultUserAgent;

        CURL* m_EasyHandle;
        CURLcode m_Response;
        std::string m_Url;
        std::vector<unsigned char> m_Buffer;
        // Protects m_Buffer, which is appended to from the download thread (write_cb)
        // while other threads may read/clear/save it
        mutable std::mutex m_BufferMutex;

        std::atomic<bool> m_Active{ false }, m_Pause{ false };
        std::atomic<curl_off_t> m_DownloadTotal{ 0 }, m_DownloadCurrent{ 0 };
        time_point_t m_StartTime;

        ImageFetcher* m_ImageFetcher{ nullptr };
        Glib::RefPtr<Gio::Cancellable> m_Cancel;

        SignalWriteType m_SignalWrite;
        Glib::Dispatcher m_SignalProgress, m_SignalFinished;
    };
}
