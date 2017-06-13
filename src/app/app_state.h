#ifndef APP_STATE
#define APP_STATE

/*This class uses a singleton model to ensure that only one instance of this class is created
* It holds all the data imformation for the gui and is the model in the MVC design pattern.*/
class app_state {
public:
  
  /*Returns a pointer to the single instance of this class creating the instance if it has not been created yet*/
  static app_state * get_pointer();

  app_state(const app_state&) = delete;
  app_state& operator=(const app_state&) = delete;


  ~app_state();
private:
  app_state();
};

#endif
