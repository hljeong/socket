#include <cassert>

#include "server.h"

using namespace sock;

using TestData = std::vector<uint8_t>;

std::vector<TestData> all_test_data = {
    {},
    {0x12, 0x34},
};

std::size_t test_data_idx = 0;

Server *s;

void callback(const uint8_t *data, const uint32_t len) {
  assert(test_data_idx < all_test_data.size());

  const TestData &test_data = all_test_data[test_data_idx++];
  assert(len == test_data.size());

  for (std::size_t i = 0; i < test_data.size(); ++i) {
    assert(data[i] == test_data[i]);
  }

  s->send(data, len);
}

int main() {
  s = new Server(callback);

  s->start();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  s->stop();
  s->close();

  assert(test_data_idx == all_test_data.size());

  delete s;
}