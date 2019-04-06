public class ProducerConsumer {
	static final int N = 100;// 定义缓冲区大小的常量
	static Producer p = new Producer();// 初始化一个新的生产者线程
	static Consumer c = new Consumer();// 初始化一个新的消费者线程
	static Our_monitor mon = new Our_monitor();// 初始化一个新的管程
	public static void main(String[] args) {
		p.start();// 生产者启动
		c.start();// 消费者启动
	}
	static class Producer extends Thread {
		public void run() {// 线程运行主代码
			int item;
			while (true) {// 生产者循环
				item = produce_item();
				mon.insert(item);
			}
		}
		private int produce_item() {
			System.out.println("生产了1个");
			return 1;
		}// 实际生产
	}
	static class Consumer extends Thread {
		public void run() {// 线程运行主代码
			int item;
			while (true) {// 消费者循环
				item = mon.remove();
				consume_item(item);
			}
		}
		private void consume_item(int item) {
			System.out.println("消费了1个");
		}// 实际消费
	}
	static class Our_monitor {// 管程
		private int buffer[] = new int[N];
		private int count = 0, lo = 0, hi = 0;// 计数器和索引
		public synchronized void insert(int val) {
			if (count == N)
				go_to_sleep();// 若缓冲区满，则进入睡眠
			buffer[hi] = val;// 向缓冲区中插入一个新的数据项
			hi = (hi + 1) % N;// 设置下一个数据项的槽
			count = count + 1;// 缓冲区的数据项又多了一项
			if (count == 1)
				notify();// 如果消费者在休眠，则将其唤醒
		}
		public synchronized int remove() {
			int val;
			if (count == 0)
				go_to_sleep();// 如果缓冲区空，进入休眠
			val = buffer[lo];// 从缓冲区中取出一个数据项
			lo = (lo + 1) % N;// 设置待取数据项的槽
			count = count - 1;// 缓冲区的数据数目减少1
			if (count == N - 1)
				notify();// 如果生产者正在休眠，则将其唤醒
			return val;
		}
		private void go_to_sleep() {
			try {wait();} catch (InterruptedException exc) {};
		}
	}
}
