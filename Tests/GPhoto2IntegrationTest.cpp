#include <GPhoto2Integration.hpp>
#include <Pbox/Logger.hpp>
#include <Pbox/SetupLogging.hpp>
#include <catch2/catch_test_macros.hpp>
#include <exec/finally.hpp>
#include <exec/static_thread_pool.hpp>

DEFINE_LOGGER(testlog);

TEST_CASE("GPhoto2 Integration", "[CameraHW]")
{
    Pbox::setupLogging();
    exec::static_thread_pool sched{3};
    auto begin = stdexec::schedule(sched.get_scheduler());

    auto final_flow = stdexec::when_all(begin | Pbox::GPhoto2::flowAutoconnect()) |
                      stdexec::let_value([&begin](Pbox::GPhoto2::Context &context) {
                          return begin | stdexec::then([&context]() {
                                     auto image = Pbox::GPhoto2::capturePreviewImage(context);
                                     LOG_DEBUG(testlog, "Got image={}", (image.has_value()));
                                     return image;
                                 }) |
                                 stdexec::then([](auto &&image) {
                                     static int count{0};
                                     LOG_DEBUG(testlog, "Has image {} count = {}", image.has_value(), count);
                                     count++;
                                     return count > 10;
                                 }) |
                                 exec::repeat_effect_until();
                          ;
                      });

    stdexec::sync_wait(std::move(final_flow));
}