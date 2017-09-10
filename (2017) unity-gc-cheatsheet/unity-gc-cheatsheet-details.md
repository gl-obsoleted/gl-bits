## Detail explanations for some entries in Unity GC Cheatsheet

- [Unity GC Cheatsheet](unity-gc-cheatsheet.md)
    + [**Details**](unity-gc-cheatsheet-details.md)
    + [References](unity-gc-cheatsheet-references.md)

### a05. 在针对 GC Alloc 做优化时，对象数量 > 引用关系复杂度 > 对象尺寸

对 [Boehm garbage collector](https://en.wikipedia.org/wiki/Boehm_garbage_collector) 而言，对象数量直接影响单次 GC 的时间开销
每个对象 90 个时钟周期左右 (大量时间是 cache-missing 所致)
算下来每秒 15M 数目的对象，也就是每毫秒标记 15000 个左右

### a07. 优化内存布局：利用“数组对 GC 而言是单一对象”这一特性

如我们有 List<Foo>，内含 100 个对象。其中，Foo 如下定义

```c#
class Foo
{
    int a;
    float b;
    bool c;
    string str;
}
```

此时内存中共有 101 个 GC 对象 (100 个 Foo + 1 个 List 内部数组) ，且为 2 级的引用关系
假如我们把数据打散成为单独的数组，如下所示：

```c#
int[] aArray;
float[] bArray;
bool[] cArray;
string[] strArray;
```

此时所有数据不变的情况下，对象数量 (对 GC 而言) 降低到了 4 个
更进一步，我们把所有的 ValueType 聚合起来

```c#
struct Foo_S
{
    int a;
    float b;
    bool c;
}
```

数据结构就成了

```c#
Foo_S[] fooArray;
string[] strArray;
```

此时所有数据不变的情况下，对象数量 (对 GC 而言) 降低到了 2 个

### c03. 避免使用枚举或 struct 做 Key 进行字典查找 (除非使用定制的 comparer)

当 Key 为用户定义的 struct 而非内建的值类型时，Dictionary 的主要接口会产生 GC Alloc
Add / ContainsKey / TryGetValue / "[ ]" 等接口都需要对传进来的 TKey 调用默认的 EqualityComparer 来判断是否相等
见 .net 代码文件 [dictionary.cs](http://referencesource.microsoft.com/#mscorlib/system/collections/generic/dictionary.cs) 的第 94 行：

```c#
this.comparer = comparer ?? EqualityComparer<TKey>.Default;
```

而 EqualityComparer 的内部调了私有的 CreateComparer() 来创建真实的 Comparer，见 [EqualityComparer.cs](http://referencesource.microsoft.com/#mscorlib/system/collections/generic/equalitycomparer.cs)：

```c#
private static EqualityComparer<T> CreateComparer() {}
```

内建类型（int/float 等等）已经实现了良好的 Equality 判断，而用户定义的 struct 则没有。很可惜上面的代码是 .Net 4.6 的最新代码，有理由推断老版本 mono 在对用户定义的 struct 调用上面的 Add / ContainsKey / TryGetValue / "[ ]" 等接口时产生了内存分配。

方案：只需要手动定义一下 Comparer 并实现 Equals() 和 GetHashCode() 即可


