class Solution:
    def trap(self, heights : list[int]) :
        n = len(heights)
        l, r = [0] * (n + 1), [0] * (n + 1)
        ans = 0
        for i in range(1, len(heights) + 1):
            l[i] = max(l[i - 1], heights[i - 1])
        for i in range(len(heights) - 1, 0, -1):
            r[i] = max(r[i + 1], heights[i])
        for i in range(len(heights)):
            ans += max(0, min(l[i + 1], r[i]) - heights[i])
        return ans

height = [2, 4, 6, 8, 9 , 3]
result = Solution()
print(result.trap(height))

#一个基本事实是，两个排序的数组，如果数组a的中间值比b的中间值大，那么比b的中间大的数至少包括a[middle:end]+b[middle:end],
#比a的中间值大的则不尽然
class MiddleSolution:
    #get sorted array a and b 's middle value. 
    def solve(a, b) :
        left = (len(a) + len(b) + 1)/2
        right = (len(a) + len(b) +2)/2
        return (getKth(a, 0, b, 0, left) + getKth(a, 0, b, 0, right))/2 
    
    #i: a的起始位置 j: b的起始位置
    def getKth(a:list[int], i, b:list[int], j, k) :
        if i >= len(a) :
            return b[j+k-1]
        if j >= len(b) :
            return a[i+k-1]
        if k == 1:
            return min(a[i], b[j])
        mid1,mid2 = min(k//2,len1)-1,min(k//2,len2)-1
        if arr1[mid1] > arr2[mid2]:
            return getKth(arr1,arr2[j+1:],k-j-1)
        else:
            return getKth(arr1[i+1:],arr2,k-i-1)
