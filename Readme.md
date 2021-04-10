## 共享内存智能指针
* **为什么需要共享内存智能指针**
> * 消息的通信在业务功能开发的过程中是必不可少的。
当出现一个模块的消息需要发送给多个模块的时候，简单的方案是对消息做多份复制，再分别发送给多个模块。这种方案对于消息内容比较少的时候是可行的，但是如果消息体的内容很大那么多分的拷贝就会造成内存极大的浪费，尤其对于图像这种类型的数据。
> * 为了解决这个问题就需要提高消息的复用性，如何提高？
答案是**智能指针**，我们可以用智能指针来对消息做计数统计，如果引用完毕，引用计数为0，那么智能指针自动调用删除器把对应的消息体删除，但是这个方案仅适用于单进程，如果是多进程的那么这个方案是不可行的，智能指针的管理对象是单个进程的，无法对于其他进程的引用计数做控制，因此我们这里需要引入共享内存+智能指针的方式来实现引用计数的统计控制。


---

* **如何实现共享内存智能指针**
>* 实现的方式就是使用共享内存+信号量+智能指针，这3项技术即可实现共享内存智能指针。
>* 共享内存用于消息的共享访问，信号量用于消息修改，引用计数增减的保护，智能指针用于进程内消息的传递。

---

* **实现方案**
> 实现方案如下图:
> * 每个进程根据共享内存名从共享内存管理器获取共享地址的key和id，并且完成attach，完成后共享内存管理器会根据attach的次数记录当前的共享内存的引用计数，同时由于计数非0，当前共享内存索引状态为使用状态。
> * 记录引用计数的同时返回共享内存在当前进程的虚拟地址和信号量，这两个参数将会由智能指针shared_ptr包裹，当本进程内使用完毕，sharedptr的use_cnt为0之后,会调用定制的析构器对共享内存引用计数减1.
> * 如果共享内存的引用计数变为0，那么销毁当前的共享内存，同时由于计数为0，当前共享内存索引状态为空闲



```mermaid
graph LR
共享内存管理器:共享内存1,ref=3==创建==>共享内存1;

进程A:主线程==创建共享内存1==>共享内存管理器:共享内存1,ref=3;
进程B:主线程==获取共享内存1==>共享内存管理器:共享内存1,ref=3;
进程C:主线程==获取共享内存1==>共享内存管理器:共享内存1,ref=3;

进程A:主线程--attach,ref=1-->共享内存1;
进程B:主线程--attach,ref=2-->共享内存1;
进程C:主线程--attach,ref=3-->共享内存1;

进程A:主线程-.shared_ptr,use_cnt=1.->进程A-线程1;
进程A:主线程-.shared_ptr,use_cnt=2.->进程A:线程2;
进程B:主线程-.shared_ptr,use_cnt=1.->进程B:线程1;
进程B:主线程-.shared_ptr,use_cnt=2.->进程B:线程2;
进程C:主线程-.shared_ptr,use_cnt=1.->进程C:线程1;
进程C:主线程-.shared_ptr,use_cnt=2.->进程C:线程2;
```
---
* 如何使用
> 共享内存管理对于一个进程来只有一份，可以通过Singleton<ShareMemMgr>模板来获取实例。
获取到的管理器实例只有可以通过AllocShareMemNode接口来获取共享内存的智能指针,对应类型为std::shared_ptr<ShareMemElem<_Type>>。
ShareMemElem模板目前重载了=,+,*这三种符号,同时提供了GetShareMemAddr和GetShareMemSema接口来获取共享内存在当前进程的虚拟地址及信号量的地址

```
#include <lemon_share_mem_mgr.hh>
#include <cstdint>

int32_t main(int32_t argc, char *arg[])
{
    //通过单例模式获取共享内存管理器
    ShareMemMgr *share_mem_mgr = Singleton<ShareMemMgr>::GetInstance();
    Print("share_mem_mgr addr is ", share_mem_mgr);

    {
        //共享内存节点的返回是一个智能指针
        const SpShareMemElem<uint32_t> &sp_share_mem = share_mem_mgr->AllocShareMemNode<uint32_t>("aaa");
        const SpShareMemElem<uint32_t> &sp_share_mem1 = share_mem_mgr->AllocShareMemNode<uint32_t>("bbb");
        *sp_share_mem1 = 100;
        Print("share addr is ", sp_share_mem->GetShareMemAddr(), "value is ", *sp_share_mem->GetShareMemAddr());
    }

    //销毁内存管理器
    Singleton<ShareMemMgr>::DestroyInstance();

    return 0;
}
```

---
