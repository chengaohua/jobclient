//
// Created by gh on 2021/9/14.
//

#ifndef TEST_QUEUE_JOBQUEUE_H_
#define TEST_QUEUE_JOBQUEUE_H_

class JobBuffer {
 public:
  JobBuffer(const JobBuffer& rhs) = delete;
  JobBuffer& operator=(const JobBuffer& rhs) = delete;

  JobBuffer(std::size_t size)
      : begin_(0), end_(0), buffered_(0), circular_buffer_(size) {
  }

  void Produce(int n) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      // 等待缓冲不为满。
      not_full_cv_.wait(lock, [=] { return buffered_ < circular_buffer_.size(); });

      // 插入新的元素，更新下标。
      circular_buffer_[end_] = n;
      end_ = (end_ + 1) % circular_buffer_.size();

      ++buffered_;
    }  // 通知前，自动解锁。

    // 通知消费者。
    not_empty_cv_.notify_one();
  }

  int Consume() {
    std::unique_lock<std::mutex> lock(mutex_);
    // 等待缓冲不为空。
    not_empty_cv_.wait(lock, [=] { return buffered_ > 0; });

    // 移除一个元素。
    int n = circular_buffer_[begin_];
    begin_ = (begin_ + 1) % circular_buffer_.size();

    --buffered_;

    // 通知前，手动解锁。
    lock.unlock();
    // 通知生产者。
    not_full_cv_.notify_one();
    return n;
  }

 private:
  std::size_t begin_;
  std::size_t end_;
  std::size_t buffered_;
  std::vector<int> circular_buffer_;
  std::condition_variable not_full_cv_;
  std::condition_variable not_empty_cv_;
  std::mutex mutex_;
};

#endif //TEST_QUEUE_JOBQUEUE_H_
