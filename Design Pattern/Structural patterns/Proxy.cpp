#include <iostream>
#include <memory>
#include <vector>
using namespace std;
using List = vector<string>;
using VideoInfo = string;
void printVec(const vector<string>& vec) {
    cout << "[";
    for (auto i{0}; i < vec.size(); ++i) {
        if (i != 0)
            cout << ", ";
        cout << vec[i];
    }
    cout << "]\n";
}

// 远程服务接口。
struct ThirdPartyTVLib {
    virtual List listVideos() const = 0;
    virtual VideoInfo getVideoInfo(uint id) const = 0;
    virtual void downloadVideo(uint id) const = 0;
    virtual ~ThirdPartyTVLib() = default;
};

// 服务连接器的具体实现。该类的方法可以向腾讯视频请求信息。请求速度取决于
// 用户和腾讯视频的互联网连接情况。如果同时发送大量请求，即使所请求的信息
// 一模一样，程序的速度依然会减慢。
class ThirdPartyTVClass : public ThirdPartyTVLib {
    inline static List list_{"v1", "v2", "v3", "v4"};
    inline static List info_{"i1", "i2", "i3", "i4"};

   public:
    List listVideos() const override { return list_; }
    VideoInfo getVideoInfo(uint id) const override { return info_.at(id); }
    void downloadVideo(uint id) const override { cout << "downloading video " << getVideoInfo(id) << ".\n"; }
};

// 为了节省网络带宽，我们可以将请求结果缓存下来并保存一段时间。但你可能无
// 法直接将这些代码放入服务类中。比如该类可能是第三方程序库的一部分或其签
// 名是`final（最终）`。因此我们会在一个实现了服务类接口的新代理类中放入
// 缓存代码。当代理类接收到真实请求后，才会将其委派给服务对象。
class CachedTVClass : public ThirdPartyTVLib {
    unique_ptr<ThirdPartyTVLib> service_;
    mutable List listCache;
    mutable VideoInfo videoInfoCache;
    mutable bool needReset;

   public:
    CachedTVClass(unique_ptr<ThirdPartyTVLib> service) : service_(std::move(service)) {}
    List listVideos() const override {
        if (listCache.empty() || needReset) {
            List tmp{service_->listVideos()};
            listCache.swap(tmp);
        }
        return listCache;
    }
    VideoInfo getVideoInfo(uint id) const override {
        if (videoInfoCache.empty() || needReset) {
            VideoInfo tmp{service_->getVideoInfo(id)};
            videoInfoCache.swap(tmp);
        }
        return videoInfoCache;
    }
    void downloadVideo(uint id) const override {
        service_->downloadVideo(id);
        needReset = true;
    }
};

// 之前直接与服务对象交互的 GUI 类不需要改变，前提是它仅通过接口与服务对
// 象交互。我们可以安全地传递一个代理对象来代替真实服务对象，因为它们都实
// 现了相同的接口。
class TVManager {
   protected:
    unique_ptr<ThirdPartyTVLib> service_;
    mutable uint prv_id_{0};

   public:
    TVManager(unique_ptr<ThirdPartyTVLib> service) : service_(std::move(service)) {}
    void renderVideoPage(uint id) const {
        auto info = service_->getVideoInfo(id);
        cout << "id " << id << "'s info: " << info << endl;
        prv_id_ = id;
    }
    void renderListPanel() const {
        auto list{service_->listVideos()};
        cout << "list: ";
        printVec(list);
    }
    void reactOnUserInput() const {
        renderListPanel();
        renderVideoPage(prv_id_);
    }
};

struct App {
    App() {
        unique_ptr<ThirdPartyTVLib> aTVService = make_unique<ThirdPartyTVClass>();
        unique_ptr<ThirdPartyTVLib> aTVProxy = make_unique<CachedTVClass>(std::move(aTVService));
        unique_ptr<TVManager> manager = make_unique<TVManager>(std::move(aTVProxy));
        manager->reactOnUserInput();
    }
};

int main() {
    App();
    return 0;
}
