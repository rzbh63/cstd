#undef access
#undef min
#undef max
#include "google\protobuf\arena.cc"
#include "google\protobuf\arenastring.cc"
#include "google\protobuf\extension_set.cc"
#include "google\protobuf\generated_message_util.cc"
#include "google\protobuf\io\coded_stream.cc"
#include "google\protobuf\io\zero_copy_stream.cc"
#include "google\protobuf\io\zero_copy_stream_impl_lite.cc"
#include "google\protobuf\message_lite.cc"
#include "google\protobuf\repeated_field.cc"
#include "google\protobuf\stubs\atomicops_internals_x86_gcc.cc"
#include "google\protobuf\stubs\atomicops_internals_x86_msvc.cc"
#include "google\protobuf\stubs\bytestream.cc"
#include "google\protobuf\stubs\common.cc"
#include "google\protobuf\stubs\int128.cc"
#include "google\protobuf\stubs\once.cc"
#include "google\protobuf\stubs\status.cc"
#include "google\protobuf\stubs\statusor.cc"
#include "google\protobuf\stubs\stringpiece.cc"
#include "google\protobuf\stubs\stringprintf.cc"
#include "google\protobuf\stubs\structurally_valid.cc"
#include "google\protobuf\stubs\strutil.cc"
#include "google\protobuf\stubs\time.cc"
#include "google\protobuf\wire_format_lite.cc"
#include "google\protobuf\any.cc"
#include "google\protobuf\any.pb.cc"
#include "google\protobuf\api.pb.cc"
#include "google\protobuf\compiler\importer.cc"
#include "google\protobuf\compiler\parser.cc"
#include "google\protobuf\descriptor.cc"
#include "google\protobuf\descriptor.pb.cc"
#include "google\protobuf\descriptor_database.cc"
#include "google\protobuf\duration.pb.cc"
#include "google\protobuf\dynamic_message.cc"
#include "google\protobuf\empty.pb.cc"
#include "google\protobuf\extension_set_heavy.cc"
#include "google\protobuf\field_mask.pb.cc"
#include "google\protobuf\generated_message_reflection.cc"
#include "google\protobuf\io\gzip_stream.cc"
#include "google\protobuf\io\printer.cc"
#include "google\protobuf\io\strtod.cc"
#include "google\protobuf\io\tokenizer.cc"
#include "google\protobuf\io\zero_copy_stream_impl.cc"
#include "google\protobuf\map_field.cc"
#include "google\protobuf\message.cc"
#include "google\protobuf\reflection_ops.cc"
#include "google\protobuf\service.cc"
#include "google\protobuf\source_context.pb.cc"
#include "google\protobuf\struct.pb.cc"
#include "google\protobuf\stubs\mathlimits.cc"
#include "google\protobuf\stubs\substitute.cc"
#include "google\protobuf\text_format.cc"
#include "google\protobuf\timestamp.pb.cc"
#include "google\protobuf\type.pb.cc"
#include "google\protobuf\unknown_field_set.cc"
#include "google\protobuf\util\field_comparator.cc"
#include "google\protobuf\util\field_mask_util.cc"
#include "google\protobuf\util\internal\datapiece.cc"
#include "google\protobuf\util\internal\default_value_objectwriter.cc"
#include "google\protobuf\util\internal\error_listener.cc"
#include "google\protobuf\util\internal\field_mask_utility.cc"
#include "google\protobuf\util\internal\json_escaping.cc"
#include "google\protobuf\util\internal\json_objectwriter.cc"
#include "google\protobuf\util\internal\json_stream_parser.cc"
#include "google\protobuf\util\internal\object_writer.cc"
#include "google\protobuf\util\internal\proto_writer.cc"
#include "google\protobuf\util\internal\protostream_objectsource.cc"
#include "google\protobuf\util\internal\protostream_objectwriter.cc"
#include "google\protobuf\util\internal\type_info.cc"
#include "google\protobuf\util\internal\type_info_test_helper.cc"
#include "google\protobuf\util\internal\utility.cc"
#include "google\protobuf\util\json_util.cc"
#include "google\protobuf\util\message_differencer.cc"
#include "google\protobuf\util\time_util.cc"
#include "google\protobuf\util\type_resolver_util.cc"
#include "google\protobuf\wire_format.cc"
#include "google\protobuf\wrappers.pb.cc"