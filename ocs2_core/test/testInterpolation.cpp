//
// Created by rgrandia on 16.05.19.
//

#include <gtest/gtest.h>
#include <ocs2_core/misc/LinearInterpolation.h>
#include <Eigen/Dense>
#include <iostream>

TEST(testLinearInterpolation, testInterpolation) {
  using Data_T = Eigen::Matrix<double, 2,1>;

  // Create data
  std::vector<double> t = {0.0, 1.0, 2.0, 3.0, 3.0, 4.0};
  std::vector<Data_T, Eigen::aligned_allocator<Data_T>> v;
  for (auto& t_k : t) {
    v.push_back(t_k * Data_T::Ones());
  }

  // Create interpolator
  ocs2::EigenLinearInterpolation<Data_T> interpolator(&t, &v);

  std::cout << "TEST DATA" << std::endl;
  for (int k=0; k<t.size(); k++){
    std::cout << "time: " << t[k] << " v: " << v[k].transpose() << std::endl;
  }

  // Test function
  auto test_interpolation = [&interpolator](double time, int index, double value){
    Data_T v_t;
    auto foundindex = interpolator.interpolate(time, v_t);
    std::cout << "time: " << time << " index: " << foundindex << " v: " << v_t.transpose() << std::endl;
    ASSERT_EQ(foundindex, index);
    ASSERT_DOUBLE_EQ(v_t(0), value);
  };

  // Before start
  test_interpolation(-1.0, -1, 0.0);
  // At start
  test_interpolation(t[0], 0, t[0]);
  // First interval
  test_interpolation(0.5*t[0] + 0.5 *t[1], 0, 0.5*t[0] + 0.5 *t[1]);
  // Boundary to second interval
  test_interpolation(t[1], 1, t[1]);
  // Last interval
  test_interpolation( 0.5*t[4] + 0.5*t[5], 4,  0.5*t[4] + 0.5*t[5]);
  // At End
  test_interpolation( t[5], 5, t[5]);
  // Beyond end
  test_interpolation( t[5] + 1.0, 5, t[5]);
}



int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}