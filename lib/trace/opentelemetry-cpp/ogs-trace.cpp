#include "opentelemetry/sdk/version/version.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/context/propagation/global_propagator.h"
#include "opentelemetry/trace/propagation/http_trace_context.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/trace/scope.h"
#include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"

#include "ogs-trace.h"


namespace ot = opentelemetry;

class HttpTextMapCarrier : public ot::context::propagation::TextMapCarrier
{
public:
    virtual ot::nostd::string_view Get(ot::nostd::string_view key) const noexcept override
    {
        auto it = headers_.find(std::string(key));
        if (it != headers_.end()) {
            return ot::nostd::string_view(it->second);
        }
        return "";
    }
    virtual void Set(ot::nostd::string_view key, ot::nostd::string_view value) noexcept override
    {
        headers_[std::string(key)] = std::string(value);
    }

    std::map<std::string, std::string> headers_;
};


struct ogs_trace_tracer_s {
    ot::nostd::shared_ptr<ot::trace::Tracer> tracer;
};
struct ogs_trace_span_s
{
    ot::nostd::shared_ptr<ot::trace::Span> span;
    char *traceparent;
};

typedef struct ogs_trace_context_s
{
    ogs_trace_tracer_t *tracer;
} ogs_trace_context_t;

static ogs_trace_context_t self;

int ogs_trace_init(const char *nf_name, const char *trace_uri)
{
    ogs_assert(self.tracer == NULL);
    
    if ((!trace_uri) || (!strlen(trace_uri)))
        return OGS_OK;

    ogs_info("Trace collector URI: '%s'", trace_uri);

    ot::exporter::otlp::OtlpHttpExporterOptions opts;
    opts.url = trace_uri;
    auto exporter = ot::exporter::otlp::OtlpHttpExporterFactory::Create(opts);

    ot::sdk::resource::ResourceAttributes resource_attributes = {
        {"service.name", nf_name},
    };
    auto resource = ot::sdk::resource::Resource::Create(resource_attributes);

    auto processor =
        ot::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));
    std::vector<std::unique_ptr<ot::sdk::trace::SpanProcessor>> processors;
    std::shared_ptr<ot::trace::TracerProvider> provider =
        ot::sdk::trace::TracerProviderFactory::Create(std::move(processor), resource);
    trace_api::Provider::SetTracerProvider(provider);

    ot::context::propagation::GlobalTextMapPropagator::SetGlobalPropagator(
        ot::nostd::shared_ptr<ot::context::propagation::TextMapPropagator>(
            new ot::trace::propagation::HttpTraceContext()));

    self.tracer = new ogs_trace_tracer_t { provider->GetTracer(nf_name) };
    ogs_assert(self.tracer);

    return OGS_OK;
}

void ogs_trace_final(void)
{
    if (self.tracer)
        delete self.tracer;
}

ogs_trace_span_t *ogs_trace_span_start(const char *name,
        ogs_trace_kind_e kind, const char *traceparent)
{
    ot::trace::StartSpanOptions options;
    HttpTextMapCarrier carrier;

    ogs_assert(name);

    if (!self.tracer)
        return NULL;

    if (kind == OGS_TRACE_KIND_CLIENT)
        options.kind = ot::trace::SpanKind::kClient;
    else if (kind == OGS_TRACE_KIND_SERVER)
        options.kind = ot::trace::SpanKind::kServer;

    if (traceparent) {
        auto propagator =
            ot::context::propagation::GlobalTextMapPropagator::GetGlobalPropagator();
        auto current_ctx = ot::context::RuntimeContext::GetCurrent();

        carrier.Set(ot::trace::propagation::kTraceParent, traceparent);

        auto new_context = propagator->Extract(carrier, current_ctx);
        options.parent = ot::trace::GetSpan(new_context)->GetContext();
    }

    auto span = self.tracer->tracer->StartSpan(name, options);
    ot::trace::Scope scope = self.tracer->tracer->WithActiveSpan(span);

    auto propagator =
            ot::context::propagation::GlobalTextMapPropagator::GetGlobalPropagator();
    auto current_ctx = ot::context::RuntimeContext::GetCurrent();
    propagator->Inject(carrier, current_ctx);

    traceparent = carrier.Get(ot::trace::propagation::kTraceParent).data();
    return new ogs_trace_span_t{ span, ogs_strdup(traceparent) };
}

void ogs_trace_span_stop(ogs_trace_span_t *span)
{
    if (!self.tracer)
        return;
    if (!span)
        return;

    span->span->End();
    if (span->traceparent)
        ogs_free(span->traceparent);
    delete span;
}

void ogs_trace_span_update_name(ogs_trace_span_t *span, char *new_name)
{
    if (!span)
        return;

    ogs_assert(new_name);

    span->span->UpdateName(new_name);
}

char *ogs_trace_span_get_traceparent(ogs_trace_span_t *span)
{
    if (!span)
        return NULL;

    return span->traceparent;
}

void ogs_trace_span_add_attr(ogs_trace_span_t *span,
        const char *key, const char *val)
{
    if (!self.tracer)
        return;
    if (!span)
        return;

    ogs_assert(key);
    ogs_assert(val);

    span->span->SetAttribute(key, val);
}

void ogs_trace_span_add_attr_int(ogs_trace_span_t *span,
        const char *key, int64_t val)
{
    if (!self.tracer)
        return;
    if (!span)
        return;

    ogs_assert(key);

    span->span->SetAttribute(key, val);
 }

void ogs_trace_span_add_attrs(ogs_trace_span_t *span,
        const char *key_prefix, ogs_hash_t *vals)
{
    ogs_hash_index_t *hi;

    if (!self.tracer)
        return;

    ogs_assert(span);
    ogs_assert(key_prefix);
    ogs_assert(vals);


    for (hi = ogs_hash_first(vals); hi; hi = ogs_hash_next(hi)) {
        char *key = NULL;
        const char *hkey = NULL;
        const char *val = NULL;

        hkey = (const char *)ogs_hash_this_key(hi);
        ogs_assert(hkey);
        val = (const char *)ogs_hash_this_val(hi);
        ogs_assert(val);

        key = ogs_msprintf("%s.%s", key_prefix, hkey);
        ogs_assert(key);
        /*
        TODO: replace '-' in keys with '_'
        */
        ogs_trace_span_add_attr(span, key, val);
        ogs_free(key);
    }
}
