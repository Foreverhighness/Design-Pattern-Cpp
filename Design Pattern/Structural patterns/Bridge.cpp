#include <iostream>
#include <memory>
using namespace std;
// “抽象部分”定义了两个类层次结构中“控制”部分的接口。它管理着一个指向“实
// 现部分”层次结构中对象的引用，并会将所有真实工作委派给该对象。
class Device;
class RemoteController;
class AdvancedRemoteControl;

class Device {
   public:
    virtual bool isEnabled() const { return isEnabled_; }
    virtual void disable() { isEnabled_ = false; }
    virtual void enable() { isEnabled_ = true; }
    virtual bool isMuted() { return isMuted_ || volume_ == 0; }
    virtual void mute() { isMuted_ = true; }
    virtual int getVolume() { return isMuted_ ? 0 : volume_; }
    virtual void setVolume(int volume) {
        volume_ = volume;
        isMuted_ = volume_ == 0;
    }
    virtual ~Device() = default;

   protected:
    bool isEnabled_;
    bool isMuted_;
    int volume_;
};
class TV : public Device {
   public:
    bool isEnabled() const override {
        cout << "TV is " << (Device::isEnabled() ? "enabled" : "disabled") << endl;
        return Device::isEnabled();
    }
    void disable() override {
        cout << "TV is going to be disabled." << endl;
        Device::disable();
    }
    void enable() override {
        cout << "TV is going to be enabled." << endl;
        Device::enable();
    }
};
class Radio : public Device {
   public:
    bool isMuted() {
        cout << "Radio is " << (Device::isMuted() ? "" : "not ") << "muted." << endl;
        return Device::isMuted();
    }
    void mute() {
        cout << "Radio is going to be muted." << endl;
        Device::mute();
    }
    int getVolume() {
        cout << "Radio's volume is " << (Device::getVolume()) << "." << endl;
        return Device::getVolume();
    }
    void setVolume(int volume) {
        cout << "Set Radio's volume to " << volume << "." << endl;
        Device::setVolume(volume);
    }
};

class RemoteController {
   public:
    explicit RemoteController(const shared_ptr<Device>& device) : device_(device) {}
    virtual ~RemoteController() = default;
    virtual void togglePower() {
        if (device_->isEnabled()) {
            device_->disable();
        } else {
            device_->enable();
        }
    };

   protected:
    shared_ptr<Device> device_;
};
class AdvancedRemoteControl : public RemoteController {
   public:
    explicit AdvancedRemoteControl(const shared_ptr<Device>& device) : RemoteController(device) {}
    void togglePower() override { RemoteController::togglePower(); }

    virtual void toggleMute() { device_->mute(); }
    bool isMuted() { return device_->isMuted(); }
    virtual int getVolume() { return device_->getVolume(); }
    virtual void setVolume(int volume) { device_->setVolume(volume); }
    virtual void mute() { device_->mute(); }
};

struct App {
    App() {
        unique_ptr<RemoteController> remote{make_unique<RemoteController>(make_shared<TV>())};
        remote->togglePower();
        auto aRemote = make_unique<AdvancedRemoteControl>(make_shared<Radio>());
        aRemote->isMuted();
        aRemote->setVolume(50);
        aRemote->getVolume();
        aRemote->isMuted();
        aRemote->mute();
        aRemote->isMuted();
        aRemote->getVolume();
    }
};
int main() {
    App();
    return 0;
}