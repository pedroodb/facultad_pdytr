struct write_data {
  string file_name<32>;
  opaque data<>;
  int amount;
};

struct read_data {
  string file_name<32>;
  int amount;
  int pos;
};

struct file_data {
  opaque data<>;
  int finished;
  int error;
};

program file_system {
  version display_ver {
    int write (write_data) = 1;
    file_data read (read_data) = 2;
  } = 1;
} = 0x20000001;