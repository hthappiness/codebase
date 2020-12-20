

var = {key1="lixi", key2="wencai",key3="junyong"}
print("mytable 的类型是 ",type(var))
for k, v in pairs(var) do
    print(k .. " - " .. v)
    --print(k, " - ", v)
end

if true or nil then
    print("It is true")
else
    print("It is false")
end

tableArr = { "orange", "banana", "apple", "pear"}
for k, v in pairs(tableArr) do
    print("key -" .. k)
end

for i=10,1,-1 do
    print(i)
end

function sum(table)
    local result = 0
    for k, v in ipairs(table) do
        result = result + v
    end
    return result
end

sumTable = {1, 2, 3, 4, 5}
result = sum(sumTable)
print(result)

array = {"Google", "Runoob", "Amazon"}

function elementIterator (collection)
   local index = 0
   local count = #collection

   print("-----loop-----", index)
   -- 闭包函数
   return function ()
      index = index + 1
      
      print("-----loop2-----", index)
   -- 闭包函数
      if index <= count
      then
         --  返回迭代器的当前元素
         return collection[index]
      end
   end
end

for element in elementIterator(array)
do
   print(element)
end

function iter (a, i)
    i = i + 1
    local v = a[i]
    if v then
       return i, v
    end
end
 
function ipairs (a)
    return iter, a, 0
end