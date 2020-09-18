#!/usr/bin/python3

#https://www.runoob.com/python3/python3-tutorial.html

#class definition
class people:
    #define basic attribute
    name = ''
    age = 0
    #define private attribute
    __weight = 0
    #define construction
    def __init__(self,n,a,w):
        self.name = n
        self.age = a
        self.__weight = w
    def speak(self):
        print("%s say: I %d years." %(self.name,self.age))
 
#single derived
class student(people):
    __privateVar = ' '
    grade = ''
    def __init__(self,n,a,w,g):
        #call parent construction
        people.__init__(self,n,a,w)
        self.grade = g
    #override parent idea
    def speak(self):
        print("%s say:I %d years old, I am reading %d grade" %(self.name,self.age,self.grade))
    
    #def __speak()  private method

#other class
class speaker():
    topic = ''
    name = ''
    def __init__(self,n,t):
        self.name = n
        self.topic = t
    def speak(self):
        print("I am %s, my speach topic is: %s"%(self.name,self.topic))
 
#multi inheritation
class sample(speaker,student):
    a =''
    def __init__(self,n,a,w,g,t):
        student.__init__(self,n,a,w,g)
        speaker.__init__(self,n,t)

s = student('ken',10,60,3)
s.speak()


#if 0 
#!/usr/bin/python3
 
class Parent:        # 定义父类
   def myMethod(self):
      print ('调用父类方法')
 
class Child(Parent): # 定义子类
   def myMethod(self):
      print ('调用子类方法')
 
c = Child()          # 子类实例
c.myMethod()         # 子类调用重写方法
super(Child,c).myMethod() #用子类对象调用父类已被覆盖的方法
#endif