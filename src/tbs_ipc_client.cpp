#include "formula_callable.hpp"
#include "tbs_ipc_client.hpp"
#include "wml_formula_callable.hpp"

namespace tbs
{

ipc_client::ipc_client(SharedMemoryPipePtr pipe) : pipe_(pipe)
{
}

void ipc_client::send_request(variant request)
{
	pipe_->write(request.write_json());
	pipe_->process();
}

void ipc_client::process()
{
	pipe_->process();

	if(!callable_) {
		return;
	}

	std::vector<std::string> msg;
	pipe_->read(msg);
	for(const std::string& m : msg) {
		variant v = game_logic::deserialize_doc_with_objects(m);

		callable_->add("message", v);
		handler_("message_received");
	}
}

	BEGIN_DEFINE_CALLABLE_NOBASE(ipc_client)
		DEFINE_FIELD(in_flight, "int")
			return variant(0);
	END_DEFINE_CALLABLE(ipc_client)


}