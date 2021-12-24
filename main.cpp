#include "Application.hpp"
#include "test-programs/TestProgram.hpp"

#include <glm/gtx/transform.hpp>

void print_mat(const glm::highp_mat4& mat) {
	for (auto i = 0; i < 4; ++i) {
		for (auto j = 0; j < 4; ++j)
			printf("%2.3f ", mat[i][j]);
		printf(" |  ");
	}
	printf("\n");
}

int main(int argc, char** argv) {
	glm::mat4x4 a;
	auto model = glm::translate(glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(1, 1, 1));
	auto view = glm::lookAt(glm::vec3(5, 5, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	auto projection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	auto mat = projection * view * model;

	print_mat(model);
	print_mat(view);
	print_mat(projection);

	Application app("rendering study", 960, 540);

	app.setProgram(new TestProgram());

	app.run();
	return 0;
}