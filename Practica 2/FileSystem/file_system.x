struct write_data {
  string file_name<32>;
  int amount;
  opaque data<>;
};

struct read_data {
  string file_name<32>;
  int pos;
  int amount;
};

struct file_data {
  opaque data<>;
};

program file_system {
  version display_ver {
    int write (write_data) = 1;
    file_data read (read_data) = 2;
  } = 1;
} = 0x20000001;