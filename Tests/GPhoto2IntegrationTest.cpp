#include <GPhoto2Integration.hpp>
#include <Pbox/SetupLogging.hpp>
#include <catch2/catch_test_macros.hpp>
#include <exec/static_thread_pool.hpp>

TEST_CASE("GPhoto2 Integration", "[CameraHW]")
{
    Pbox::setupLogging();
    exec::static_thread_pool sched{1};
    auto begin = stdexec::schedule(sched.get_scheduler());
    auto autoconnect = Pbox::GPhoto2::flowAutoconnect();

    auto flow = begin | std::move(autoconnect);

    stdexec::sync_wait(std::move(flow));
}
