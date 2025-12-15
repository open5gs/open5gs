/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "location-determination.h"
#include "namf-path.h"
#include "nrppa-types.h"
#include "cell-database.h"
#include "nlmf-handler.h"
#include "sbi-path.h"
#include <math.h>
#include "openapi/model/ncgi.h"

int lmf_location_determine(lmf_location_request_t *location_request)
{
    int rv;

    ogs_assert(location_request);

    ogs_assert(location_request->positioning_method);

    /* Route based on positioning method */
    if (strcmp(location_request->positioning_method, "CELLID") == 0) {
        /* Check if we need to query AMF for Cell ID */
        OpenAPI_input_data_t *input_data = location_request->input_message->InputData;
        if (!input_data || (!input_data->ncgi && !input_data->ecgi)) {
            /* No cell info provided - request from AMF via async API */
            ogs_info("[%s] Starting location determination (CELLID) - requesting Cell ID from AMF",
                    location_request->supi ? location_request->supi : "Unknown");
            
            /* Send async request to AMF for location info */
            rv = lmf_amf_send_location_info_request(location_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] lmf_amf_send_location_info_request() failed",
                        location_request->supi);
                
                /* Send error response to client */
                ogs_sbi_stream_t *stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
                if (stream) {
                    ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE,
                            NULL, "AMF communication failed",
                            "Unable to request Cell ID from AMF", NULL);
                }
                
                lmf_location_request_remove(location_request);
                return rv;
            }
            
            /* Response will be handled asynchronously in lmf_namf_handler_location_info_response */
            return OGS_OK;
        } else {
            /* Cell info provided - proceed directly */
            ogs_info("[%s] Starting location determination (CELLID) - using provided cell info",
                    location_request->supi ? location_request->supi : "Unknown");
            
            rv = lmf_location_determine_cellid(location_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] lmf_location_determine_cellid() failed",
                        location_request->supi);
                /* Error response already sent by lmf_location_determine_cellid() */
                return rv;
            }
            
            return OGS_OK;
        }
        
    } else if (strcmp(location_request->positioning_method, "ECID") == 0) {
        /* For ECID, we need to request NRPPa measurement from AMF */
        ogs_info("[%s] Starting location determination (ECID)",
                location_request->supi ? location_request->supi : "Unknown");

        /* Send NRPPa measurement request to AMF with all supported measurements */
        rv = lmf_amf_send_nrppa_measurement_request(location_request);
        if (rv != OGS_OK) {
            ogs_error("[%s] lmf_amf_send_nrppa_measurement_request() failed",
                    location_request->supi);
            
            /* Send error response to client */
            ogs_sbi_stream_t *stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
            if (stream) {
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE,
                        NULL, "AMF communication failed",
                        "Unable to send NRPPa measurement request to AMF", NULL);
            } else {
                ogs_error("[%s] Stream ID=%d not found for error response",
                        location_request->supi, location_request->stream_id);
            }
            
            lmf_location_request_remove(location_request);
            return rv;
        }

        return OGS_OK;
    } else {
        ogs_error("[%s] Unsupported positioning method: %s",
                location_request->supi,
                location_request->positioning_method);
        
        /* Send error response to client */
        ogs_sbi_stream_t *stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "Unsupported positioning method",
                    location_request->positioning_method, NULL);
        } else {
            ogs_error("[%s] Stream ID=%d not found for error response",
                    location_request->supi, location_request->stream_id);
        }
        
        lmf_location_request_remove(location_request);
        return OGS_ERROR;
    }
}

/*
 * Convert RSRP (0.1 dBm units) to linear power (mW) for weighting
 */
static double rsrp_to_linear_power(int16_t rsrp_dbm_01)
{
    double rsrp_dbm = rsrp_dbm_01 / 10.0;
    /* Convert dBm to mW: P_mW = 10^(P_dBm / 10) */
    return pow(10.0, rsrp_dbm / 10.0);
}

/*
 * Calculate distance from Timing Advance
 * TA is in 16 Ts units, where Ts = 1/(48000 * 2048) seconds
 * Distance = c * TA * 16 * Ts / 2 (one-way)
 */
static double ta_to_distance_meters(uint16_t ta)
{
    const double c = 299792458.0;  /* Speed of light in m/s */
    const double ts = 1.0 / (48000.0 * 2048.0);  /* Sampling period */
    const double ta_seconds = ta * 16.0 * ts;
    return (c * ta_seconds) / 2.0;  /* One-way distance */
}

int lmf_location_determine_ecid(
        lmf_location_request_t *location_request,
        lmf_nrppa_ecid_response_t *ecid_response)
{
    int rv;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_response_t *response = NULL;
    lmf_cell_info_t *serving_cell_info = NULL;
    ogs_nr_cgi_t serving_ncgi;
    double final_lat = 0.0, final_lon = 0.0, final_alt = 0.0;
    double total_weight = 0.0;
    double uncertainty_radius = 0.0;
    int i;
    OpenAPI_location_data_t *location_data = NULL;
    OpenAPI_geographic_area_t *geographic_area = NULL;
    OpenAPI_geographical_coordinates_t *coordinates = NULL;
    OpenAPI_supported_gad_shapes_t *shape = NULL;
    OpenAPI_ncgi_t *ncgi_openapi = NULL;
    OpenAPI_plmn_id_t *plmn_id_openapi = NULL;

    ogs_assert(location_request);
    ogs_assert(ecid_response);

    ogs_info("[%s] ECID location determination from measurements",
            location_request->supi ? location_request->supi : "Unknown");

    /* Build serving cell NCGI from ECID response */
    memset(&serving_ncgi, 0, sizeof(serving_ncgi));
    serving_ncgi.cell_id = ecid_response->serving_cell.ncgi;
    /* Use default PLMN if not available - should be improved */
    ogs_plmn_id_build(&serving_ncgi.plmn_id, 999, 70, 2);

    /* Lookup serving cell in database */
    serving_cell_info = lmf_cell_database_find_by_ncgi(&serving_ncgi);
    if (!serving_cell_info) {
        ogs_error("[%s] Serving cell NCGI=%llx not found in database",
                location_request->supi,
                (unsigned long long)ecid_response->serving_cell.ncgi);
        
        /* Try to find by cell_id only */
        serving_cell_info = lmf_cell_database_find_by_cell_id(
                ecid_response->serving_cell.ncgi);
    }

    if (!serving_cell_info) {
        ogs_error("[%s] Cannot calculate location: serving cell not in database",
                location_request->supi);
        
        /* Send error response */
        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    NULL, "Serving cell not found in database",
                    "The serving cell is not configured in LMF cell database", NULL);
        }
        return OGS_ERROR;
    }

    ogs_info("[%s] Serving cell found: lat=%.6f, lon=%.6f, alt=%.1f",
            location_request->supi,
            serving_cell_info->latitude,
            serving_cell_info->longitude,
            serving_cell_info->altitude);

    /* Calculate weighted position from serving cell and neighbors */
    /* Weight by RSRP (higher RSRP = stronger signal = closer = higher weight) */
    
    /* Serving cell weight */
    double serving_weight = 1.0;  /* Default weight */
    if (ecid_response->serving_cell.rsrp != 0) {
        serving_weight = rsrp_to_linear_power(ecid_response->serving_cell.rsrp);
    }
    
    final_lat += serving_cell_info->latitude * serving_weight;
    final_lon += serving_cell_info->longitude * serving_weight;
    final_alt += serving_cell_info->altitude * serving_weight;
    total_weight += serving_weight;

    ogs_info("[%s] Serving cell: RSRP=%d (%.1f dBm), weight=%.3e",
            location_request->supi,
            ecid_response->serving_cell.rsrp,
            ecid_response->serving_cell.rsrp / 10.0,
            serving_weight);

    /* Add neighbor cells */
    for (i = 0; i < ecid_response->num_neighbor_cells; i++) {
        ogs_nr_cgi_t neighbor_ncgi;
        lmf_cell_info_t *neighbor_cell_info = NULL;
        double neighbor_weight;

        memset(&neighbor_ncgi, 0, sizeof(neighbor_ncgi));
        neighbor_ncgi.cell_id = ecid_response->neighbor_cells[i].ncgi;
        ogs_plmn_id_build(&neighbor_ncgi.plmn_id, 999, 70, 2);

        neighbor_cell_info = lmf_cell_database_find_by_ncgi(&neighbor_ncgi);
        if (!neighbor_cell_info) {
            neighbor_cell_info = lmf_cell_database_find_by_cell_id(
                    ecid_response->neighbor_cells[i].ncgi);
        }

        if (neighbor_cell_info) {
            /* Weight by RSRP */
            neighbor_weight = 1.0;
            if (ecid_response->neighbor_cells[i].rsrp != 0) {
                neighbor_weight = rsrp_to_linear_power(
                        ecid_response->neighbor_cells[i].rsrp);
            }

            final_lat += neighbor_cell_info->latitude * neighbor_weight;
            final_lon += neighbor_cell_info->longitude * neighbor_weight;
            final_alt += neighbor_cell_info->altitude * neighbor_weight;
            total_weight += neighbor_weight;

            ogs_debug("[%s] Neighbor[%d]: NCGI=%llx, RSRP=%d, weight=%.3e",
                    location_request->supi, i,
                    (unsigned long long)ecid_response->neighbor_cells[i].ncgi,
                    ecid_response->neighbor_cells[i].rsrp,
                    neighbor_weight);
        }
    }

    /* Normalize weighted position */
    if (total_weight > 0.0) {
        final_lat /= total_weight;
        final_lon /= total_weight;
        final_alt /= total_weight;
    } else {
        /* Fallback: use serving cell position */
        final_lat = serving_cell_info->latitude;
        final_lon = serving_cell_info->longitude;
        final_alt = serving_cell_info->altitude;
    }

    /* Calculate uncertainty radius */
    /* Start with cell radius */
    uncertainty_radius = serving_cell_info->cell_radius;

    /* Add timing advance distance uncertainty */
    if (ecid_response->serving_cell.timing_advance > 0) {
        double ta_distance = ta_to_distance_meters(
                ecid_response->serving_cell.timing_advance);
        /* Use TA distance as minimum uncertainty */
        if (ta_distance > uncertainty_radius) {
            uncertainty_radius = ta_distance;
        }
        ogs_debug("[%s] Timing Advance: TA=%u, distance=%.1f m",
                location_request->supi,
                ecid_response->serving_cell.timing_advance,
                ta_distance);
    }

    /* Add some margin for measurement inaccuracy (20% of radius) */
    uncertainty_radius *= 1.2;

    /* Minimum uncertainty: 50 meters */
    if (uncertainty_radius < 50.0) {
        uncertainty_radius = 50.0;
    }

    ogs_info("[%s] Calculated location: lat=%.6f, lon=%.6f, alt=%.1f, "
            "uncertainty=%.0f m",
            location_request->supi,
            final_lat, final_lon, final_alt, uncertainty_radius);

    /* Build LocationData response */
    memset(&message, 0, sizeof(message));
    
    /* Create NCGI for response */
    ncgi_openapi = ogs_calloc(1, sizeof(OpenAPI_ncgi_t));
    ogs_assert(ncgi_openapi);
    
    plmn_id_openapi = ogs_calloc(1, sizeof(OpenAPI_plmn_id_t));
    ogs_assert(plmn_id_openapi);
    
    /* Convert PLMN ID */
    plmn_id_openapi->mcc = ogs_plmn_id_mcc_string(&serving_ncgi.plmn_id);
    plmn_id_openapi->mnc = ogs_plmn_id_mnc_string(&serving_ncgi.plmn_id);
    
    ncgi_openapi->plmn_id = plmn_id_openapi;
    ncgi_openapi->nr_cell_id = ogs_msprintf("%llx",
            (unsigned long long)ecid_response->serving_cell.ncgi);
    
    /* Create geographic area with all parameters */
    shape = OpenAPI_supported_gad_shapes_create();
    ogs_assert(shape);
    
    coordinates = OpenAPI_geographical_coordinates_create(final_lon, final_lat);
    ogs_assert(coordinates);
    
    geographic_area = OpenAPI_geographic_area_create(
        shape,                      /* shape */
        coordinates,                /* point */
        (float)uncertainty_radius,  /* uncertainty */
        NULL,                       /* uncertainty_ellipse */
        0,                          /* confidence */
        NULL,                       /* point_list */
        final_alt,                  /* altitude */
        10.0f,                      /* uncertainty_altitude */
        0,                          /* inner_radius */
        0.0f,                       /* uncertainty_radius */
        0,                          /* offset_angle */
        0                           /* included_angle */
    );
    ogs_assert(geographic_area);
    
    /* Create LocationData */
    location_data = OpenAPI_location_data_create(
            geographic_area,
            NULL,  /* accuracy_fulfilment_indicator */
            0,     /* is_age_of_location_estimate */
            0,     /* age_of_location_estimate */
            NULL,  /* timestamp_of_location_estimate */
            NULL,  /* velocity_estimate */
            NULL,  /* civic_address */
            NULL,  /* local_location_estimate */
            NULL,  /* positioning_data_list */
            NULL,  /* gnss_positioning_data_list */
            NULL,  /* ecgi */
            ncgi_openapi,
            1,     /* is_altitude */
            final_alt,
            0,     /* is_barometric_pressure */
            0,     /* barometric_pressure */
            NULL,  /* serving_lmf_identification */
            NULL,  /* ue_positioning_cap */
            NULL,  /* ue_area_ind */
            NULL,  /* supported_features */
            NULL   /* achieved_qos */
    );
    ogs_assert(location_data);

    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NLMF_LOC;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = (char *)"determine-location";
    message.LocationData = location_data;

    ogs_info("[%s] LocationData prepared: lat=%.6f, lon=%.6f, uncertainty=%.0f",
            location_request->supi, final_lat, final_lon, uncertainty_radius);
    ogs_info("  NCGI: plmn=%s/%s, cell=%s",
            plmn_id_openapi->mcc, plmn_id_openapi->mnc, ncgi_openapi->nr_cell_id);

    /* Build comprehensive JSON response with all location data */
    cJSON *root = cJSON_CreateObject();
    ogs_assert(root);
    
    cJSON *location_obj = cJSON_CreateObject();
    
    /* UE Identity (SUPI/IMSI) */
    if (location_request->supi) {
        cJSON_AddStringToObject(location_obj, "supi", location_request->supi);
    }
    
    /* Geographic coordinates (calculated position from ECID measurements) */
    cJSON_AddNumberToObject(location_obj, "latitude", final_lat);
    cJSON_AddNumberToObject(location_obj, "longitude", final_lon);
    cJSON_AddNumberToObject(location_obj, "altitude", final_alt);
    
    /* Uncertainty (critical for positioning accuracy assessment) */
    cJSON_AddNumberToObject(location_obj, "uncertaintyRadius", uncertainty_radius);
    cJSON_AddNumberToObject(location_obj, "uncertaintyAltitude", 10.0);
    cJSON_AddNumberToObject(location_obj, "confidence", 68);  /* ~68% confidence circle */
    
    /* Serving cell information */
    cJSON *ncgi_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(ncgi_obj, "mcc", plmn_id_openapi->mcc);
    cJSON_AddStringToObject(ncgi_obj, "mnc", plmn_id_openapi->mnc);
    cJSON_AddStringToObject(ncgi_obj, "nrCellId", ncgi_openapi->nr_cell_id);
    cJSON_AddItemToObject(location_obj, "ncgi", ncgi_obj);
    
    /* Cell ID as separate field for convenience (derived from NCGI) */
    cJSON_AddStringToObject(location_obj, "cellId", ncgi_openapi->nr_cell_id);
    
    /* Positioning method and quality */
    cJSON_AddStringToObject(location_obj, "positioningMethod", "ECID");
    
    /* RSRP measurement (always present) */
    cJSON_AddNumberToObject(location_obj, "rsrp", ecid_response->serving_cell.rsrp / 10.0);  /* dBm */
    
    /* Timing Advance measurement (always include, 0 is valid) */
    cJSON_AddNumberToObject(location_obj, "timingAdvance", 
                            ecid_response->serving_cell.timing_advance);
    /* Add approximate distance based on TA (TA × 156 meters) */
    uint32_t distance_m = (uint32_t)ecid_response->serving_cell.timing_advance * 156;
    cJSON_AddNumberToObject(location_obj, "timingAdvanceDistance", distance_m);
    
    /* Angle of Arrival measurement (if present) */
    if (ecid_response->serving_cell.angle_of_arrival != 0) {
        /* Convert from degrees*2 to degrees */
        double azimuth_degrees = ecid_response->serving_cell.angle_of_arrival / 2.0;
        cJSON_AddNumberToObject(location_obj, "azimuthAngleOfArrival", azimuth_degrees);
    }
    
    cJSON_AddNumberToObject(location_obj, "timestamp", (double)ecid_response->measurement_timestamp);
    
    cJSON_AddItemToObject(root, "locationData", location_obj);
    
    /* Optional debug/diagnostic information (3GPP-compliant optional field) */
    cJSON *debug_obj = cJSON_CreateObject();
    if (debug_obj) {
        cJSON_AddNumberToObject(debug_obj, "measurementId", (double)ecid_response->measurement_id);
        cJSON_AddNumberToObject(debug_obj, "numNeighborCells", (double)ecid_response->num_neighbor_cells);
        cJSON_AddNumberToObject(debug_obj, "totalWeight", total_weight);
        cJSON_AddNumberToObject(debug_obj, "servingCellWeight", serving_weight);
        cJSON_AddNumberToObject(debug_obj, "cellRadius", serving_cell_info->cell_radius);
        
        /* Cell database lookup status */
        cJSON_AddBoolToObject(debug_obj, "servingCellFound", serving_cell_info != NULL);
        if (serving_cell_info) {
            cJSON_AddNumberToObject(debug_obj, "servingCellLatitude", serving_cell_info->latitude);
            cJSON_AddNumberToObject(debug_obj, "servingCellLongitude", serving_cell_info->longitude);
        }
        
        /* Timing advance info */
        if (ecid_response->serving_cell.timing_advance > 0) {
            double ta_distance = ta_to_distance_meters(ecid_response->serving_cell.timing_advance);
            cJSON_AddNumberToObject(debug_obj, "timingAdvanceDistanceMeters", ta_distance);
        }
        
        /* Angle of arrival info */
        if (ecid_response->serving_cell.angle_of_arrival != 0) {
            double azimuth_degrees = ecid_response->serving_cell.angle_of_arrival / 2.0;
            cJSON_AddNumberToObject(debug_obj, "azimuthAngleOfArrivalDegrees", azimuth_degrees);
        }
        
        /* Calculation method */
        if (total_weight > 0.0) {
            cJSON_AddStringToObject(debug_obj, "calculationMethod", "weighted_average");
        } else {
            cJSON_AddStringToObject(debug_obj, "calculationMethod", "cell_center");
        }
        
        /* Add to root - AFTER locationData */
        cJSON_AddItemToObject(root, "debug", debug_obj);
    }
    
    /* Use unformatted JSON for consistency with Open5GS and better jq compatibility */
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    
    if (!json_str) {
        ogs_error("[%s] cJSON_PrintUnformatted failed", location_request->supi);
        ogs_sbi_message_free(&message);
        return OGS_ERROR;
    }
    
    ogs_info("[%s] LocationData JSON: %s", location_request->supi, json_str);
    
    /* Build HTTP response */
    response = ogs_sbi_response_new();
    ogs_assert(response);
    response->status = OGS_SBI_HTTP_STATUS_OK;
    response->http.content = json_str;
    response->http.content_length = strlen(json_str);
    ogs_sbi_header_set(response->http.headers, "Content-Type", "application/json");

    /* Free message (which frees LocationData) - response has copied the data */
    ogs_sbi_message_free(&message);

    /* Find stream for async response */
    stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
    if (!stream) {
        ogs_error("[%s] Stream ID=%d not found for async response",
                location_request->supi, location_request->stream_id);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    /* Send async response */
    rv = ogs_sbi_server_send_response(stream, response);
    if (!rv) {
        ogs_error("[%s] ogs_sbi_server_send_response() failed",
                location_request->supi);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    ogs_info("[%s] Location response sent successfully",
            location_request->supi);

    /* Clean up location request */
    lmf_location_request_remove(location_request);

    return OGS_OK;
}

int lmf_location_determine_cellid(lmf_location_request_t *location_request)
{
    int rv;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_response_t *response = NULL;
    lmf_cell_info_t *cell_info = NULL;
    ogs_nr_cgi_t ncgi;
    OpenAPI_input_data_t *input_data = NULL;
    OpenAPI_ncgi_t *ncgi_openapi_input = NULL;
    OpenAPI_ecgi_t *ecgi_openapi_input = NULL;
    OpenAPI_location_data_t *location_data = NULL;
    OpenAPI_geographic_area_t *geographic_area = NULL;
    OpenAPI_geographical_coordinates_t *coordinates = NULL;
    OpenAPI_supported_gad_shapes_t *shape = NULL;
    OpenAPI_ncgi_t *ncgi_openapi = NULL;
    OpenAPI_plmn_id_t *plmn_id_openapi = NULL;
    double final_lat = 0.0, final_lon = 0.0, final_alt = 0.0;
    double uncertainty_radius = 0.0;
    uint64_t cell_id = 0;
    uint16_t mcc = 0, mnc = 0;
    uint16_t mnc_len = 2;
    bool is_ncgi = false;

    ogs_assert(location_request);
    ogs_assert(location_request->input_message);

    ogs_info("[%s] CELLID location determination from provided cell information",
            location_request->supi ? location_request->supi : "Unknown");

    /* Extract InputData from stored message */
    input_data = location_request->input_message->InputData;
    if (!input_data) {
        ogs_error("[%s] No InputData in stored message",
                location_request->supi);
        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    NULL, "Internal server error",
                    "InputData not available", NULL);
            /* Don't clean up here - let the handler do it to avoid use-after-free */
            /* Just mark input_message as NULL so handler knows error was sent */
            ogs_sbi_message_free(location_request->input_message);
            ogs_free(location_request->input_message);
            location_request->input_message = NULL;
        }
        return OGS_ERROR;
    }

    /* Extract NCGI or ECGI from InputData */
    ncgi_openapi_input = input_data->ncgi;
    ecgi_openapi_input = input_data->ecgi;

    if (!ncgi_openapi_input && !ecgi_openapi_input) {
        /* Check if we already have NCGI from AMF (from async response) */
        if (location_request->ncgi_from_amf) {
            /* Use NCGI retrieved from AMF */
            ncgi_openapi_input = location_request->ncgi_from_amf;
            ogs_info("[%s] Using NCGI retrieved from AMF: PLMN=%s/%s, CellID=%s",
                    location_request->supi,
                    ncgi_openapi_input->plmn_id ?
                        (ncgi_openapi_input->plmn_id->mcc ? ncgi_openapi_input->plmn_id->mcc : "N/A") : "N/A",
                    ncgi_openapi_input->plmn_id ?
                        (ncgi_openapi_input->plmn_id->mnc ? ncgi_openapi_input->plmn_id->mnc : "N/A") : "N/A",
                    ncgi_openapi_input->nr_cell_id ? ncgi_openapi_input->nr_cell_id : "N/A");
            /* Note: ncgi_from_amf will be freed in location_request cleanup */
        } else {
            /* No NCGI from AMF yet - should not happen, this function is called after async response */
            ogs_error("[%s] No NCGI/ECGI provided and no NCGI from AMF available",
                    location_request->supi);
            stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
            if (stream) {
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        NULL, "Internal server error",
                        "Cell information not available", NULL);
            }
            ogs_sbi_message_free(location_request->input_message);
            ogs_free(location_request->input_message);
            location_request->input_message = NULL;
            return OGS_ERROR;
        }
    }

    /* Process NCGI (5G NR) or ECGI (4G LTE) */
    memset(&ncgi, 0, sizeof(ncgi));
    
    if (ncgi_openapi_input) {
        is_ncgi = true;
        ogs_info("[%s] Using NCGI for CELLID positioning",
                location_request->supi);

        /* Extract PLMN ID from NCGI */
        if (ncgi_openapi_input->plmn_id) {
            if (ncgi_openapi_input->plmn_id->mcc) {
                mcc = atoi(ncgi_openapi_input->plmn_id->mcc);
            }
            if (ncgi_openapi_input->plmn_id->mnc) {
                mnc = atoi(ncgi_openapi_input->plmn_id->mnc);
                mnc_len = strlen(ncgi_openapi_input->plmn_id->mnc);
            }
        }

        /* Extract cell ID from NCGI (hex string without 0x prefix) */
        if (ncgi_openapi_input->nr_cell_id) {
            cell_id = strtoull(ncgi_openapi_input->nr_cell_id, NULL, 16);
            ogs_info("[%s] NCGI cell_id=%llx (from string: %s)",
                    location_request->supi,
                    (unsigned long long)cell_id,
                    ncgi_openapi_input->nr_cell_id);
        } else {
            ogs_error("[%s] NCGI missing nr_cell_id",
                    location_request->supi);
            stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
            if (stream) {
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        NULL, "Invalid NCGI",
                        "NCGI missing nr_cell_id", NULL);
                /* Don't clean up here - let the handler do it to avoid use-after-free */
                ogs_sbi_message_free(location_request->input_message);
                ogs_free(location_request->input_message);
                location_request->input_message = NULL;
            }
            return OGS_ERROR;
        }
    } else if (ecgi_openapi_input) {
        is_ncgi = false;
        ogs_info("[%s] Using ECGI for CELLID positioning (LTE)",
                location_request->supi);

        /* Extract PLMN ID from ECGI */
        if (ecgi_openapi_input->plmn_id) {
            if (ecgi_openapi_input->plmn_id->mcc) {
                mcc = atoi(ecgi_openapi_input->plmn_id->mcc);
            }
            if (ecgi_openapi_input->plmn_id->mnc) {
                mnc = atoi(ecgi_openapi_input->plmn_id->mnc);
                mnc_len = strlen(ecgi_openapi_input->plmn_id->mnc);
            }
        }

        /* Extract cell ID from ECGI (hex string without 0x prefix) */
        if (ecgi_openapi_input->eutra_cell_id) {
            cell_id = strtoull(ecgi_openapi_input->eutra_cell_id, NULL, 16);
            ogs_info("[%s] ECGI cell_id=%llx (from string: %s)",
                    location_request->supi,
                    (unsigned long long)cell_id,
                    ecgi_openapi_input->eutra_cell_id);
        } else {
            ogs_error("[%s] ECGI missing eutra_cell_id",
                    location_request->supi);
            stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
            if (stream) {
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        NULL, "Invalid ECGI",
                        "ECGI missing eutra_cell_id", NULL);
                /* Don't clean up here - let the handler do it to avoid use-after-free */
                ogs_sbi_message_free(location_request->input_message);
                ogs_free(location_request->input_message);
                location_request->input_message = NULL;
            }
            return OGS_ERROR;
        }
    }

    /* Build internal NCGI structure */
    ncgi.cell_id = cell_id;
    if (mcc > 0 && mnc > 0) {
        ogs_plmn_id_build(&ncgi.plmn_id, mcc, mnc, mnc_len);
    } else {
        /* Use default PLMN if not specified */
        ogs_warn("[%s] PLMN not specified, using default 999/70",
                location_request->supi);
        ogs_plmn_id_build(&ncgi.plmn_id, 999, 70, 2);
    }

    /* Lookup cell in database */
    cell_info = lmf_cell_database_find_by_ncgi(&ncgi);
    if (!cell_info) {
        /* Try fallback: lookup by cell_id only (if PLMN doesn't match) */
        ogs_debug("[%s] Cell not found by NCGI, trying cell_id-only lookup",
                location_request->supi);
        cell_info = lmf_cell_database_find_by_cell_id(cell_id);
    }

    if (!cell_info) {
        ogs_error("[%s] Cell not found in database: NCGI=%llx [PLMN:%06x,CELL:%llx]",
                location_request->supi,
                (unsigned long long)cell_id,
                ogs_plmn_id_hexdump(&ncgi.plmn_id),
                (unsigned long long)ncgi.cell_id);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    NULL, "Cell not found in database",
                    "The specified cell is not configured in LMF cell database", NULL);
            /* Don't clean up here - let the handler do it to avoid use-after-free */
            ogs_sbi_message_free(location_request->input_message);
            ogs_free(location_request->input_message);
            location_request->input_message = NULL;
        }
        return OGS_ERROR;
    }

    ogs_info("[%s] Cell found in database: lat=%.6f, lon=%.6f, alt=%.1f, radius=%um",
            location_request->supi,
            cell_info->latitude,
            cell_info->longitude,
            cell_info->altitude,
            cell_info->cell_radius);

    /* Use cell position directly (CELLID positioning uses cell center) */
    final_lat = cell_info->latitude;
    final_lon = cell_info->longitude;
    final_alt = cell_info->altitude;
    uncertainty_radius = (double)cell_info->cell_radius;

    /* Minimum uncertainty: 50 meters */
    if (uncertainty_radius < 50.0) {
        uncertainty_radius = 50.0;
    }

    ogs_info("[%s] Calculated location (CELLID): lat=%.6f, lon=%.6f, alt=%.1f, "
            "uncertainty=%.0f m",
            location_request->supi,
            final_lat, final_lon, final_alt, uncertainty_radius);

    /* Build LocationData response */
    memset(&message, 0, sizeof(message));
    
    /* Create NCGI for response */
    ncgi_openapi = ogs_calloc(1, sizeof(OpenAPI_ncgi_t));
    ogs_assert(ncgi_openapi);
    
    plmn_id_openapi = ogs_calloc(1, sizeof(OpenAPI_plmn_id_t));
    ogs_assert(plmn_id_openapi);
    
    /* Convert PLMN ID */
    plmn_id_openapi->mcc = ogs_plmn_id_mcc_string(&ncgi.plmn_id);
    plmn_id_openapi->mnc = ogs_plmn_id_mnc_string(&ncgi.plmn_id);
    
    ncgi_openapi->plmn_id = plmn_id_openapi;
    ncgi_openapi->nr_cell_id = ogs_msprintf("%llx",
            (unsigned long long)ncgi.cell_id);
    
    /* Create geographic area */
    shape = OpenAPI_supported_gad_shapes_create();
    ogs_assert(shape);
    
    coordinates = OpenAPI_geographical_coordinates_create(final_lon, final_lat);
    ogs_assert(coordinates);
    
    geographic_area = OpenAPI_geographic_area_create(
        shape,                      /* shape */
        coordinates,                /* point */
        (float)uncertainty_radius,  /* uncertainty */
        NULL,                       /* uncertainty_ellipse */
        0,                          /* confidence */
        NULL,                       /* point_list */
        final_alt,                  /* altitude */
        10.0f,                      /* uncertainty_altitude */
        0,                          /* inner_radius */
        0.0f,                       /* uncertainty_radius */
        0,                          /* offset_angle */
        0                           /* included_angle */
    );
    ogs_assert(geographic_area);
    
    /* Create LocationData */
    location_data = OpenAPI_location_data_create(
            geographic_area,
            NULL,  /* accuracy_fulfilment_indicator */
            0,     /* is_age_of_location_estimate */
            0,     /* age_of_location_estimate */
            NULL,  /* timestamp_of_location_estimate */
            NULL,  /* velocity_estimate */
            NULL,  /* civic_address */
            NULL,  /* local_location_estimate */
            NULL,  /* positioning_data_list */
            NULL,  /* gnss_positioning_data_list */
            NULL,  /* ecgi */
            ncgi_openapi,
            1,     /* is_altitude */
            final_alt,
            0,     /* is_barometric_pressure */
            0,     /* barometric_pressure */
            NULL,  /* serving_lmf_identification */
            NULL,  /* ue_positioning_cap */
            NULL,  /* ue_area_ind */
            NULL,  /* supported_features */
            NULL   /* achieved_qos */
    );
    ogs_assert(location_data);

    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NLMF_LOC;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = (char *)"determine-location";
    message.LocationData = location_data;

    ogs_info("[%s] LocationData prepared: lat=%.6f, lon=%.6f, uncertainty=%.0f",
            location_request->supi, final_lat, final_lon, uncertainty_radius);
    ogs_info("  NCGI: plmn=%s/%s, cell=%s",
            plmn_id_openapi->mcc, plmn_id_openapi->mnc, ncgi_openapi->nr_cell_id);

    /* Build JSON response */
    cJSON *root = cJSON_CreateObject();
    ogs_assert(root);
    
    cJSON *location_obj = cJSON_CreateObject();
    
    /* UE Identity (SUPI/IMSI) */
    if (location_request->supi) {
        cJSON_AddStringToObject(location_obj, "supi", location_request->supi);
    }
    
    /* Geographic coordinates (cell center position) */
    cJSON_AddNumberToObject(location_obj, "latitude", final_lat);
    cJSON_AddNumberToObject(location_obj, "longitude", final_lon);
    cJSON_AddNumberToObject(location_obj, "altitude", final_alt);
    
    /* Uncertainty (cell radius) */
    cJSON_AddNumberToObject(location_obj, "uncertaintyRadius", uncertainty_radius);
    cJSON_AddNumberToObject(location_obj, "uncertaintyAltitude", 10.0);
    cJSON_AddNumberToObject(location_obj, "confidence", 68);  /* ~68% confidence circle */
    
    /* Serving cell information */
    cJSON *ncgi_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(ncgi_obj, "mcc", plmn_id_openapi->mcc);
    cJSON_AddStringToObject(ncgi_obj, "mnc", plmn_id_openapi->mnc);
    cJSON_AddStringToObject(ncgi_obj, "nrCellId", ncgi_openapi->nr_cell_id);
    cJSON_AddItemToObject(location_obj, "ncgi", ncgi_obj);
    
    /* Cell ID as separate field for convenience */
    cJSON_AddStringToObject(location_obj, "cellId", ncgi_openapi->nr_cell_id);
    
    /* Positioning method */
    cJSON_AddStringToObject(location_obj, "positioningMethod", "CELLID");
    
    /* Timestamp (current time in milliseconds) */
    uint64_t timestamp = ogs_time_now() / 1000;  /* Convert microseconds to milliseconds */
    cJSON_AddNumberToObject(location_obj, "timestamp", (double)timestamp);
    
    cJSON_AddItemToObject(root, "locationData", location_obj);
    
    /* Optional debug information */
    cJSON *debug_obj = cJSON_CreateObject();
    if (debug_obj) {
        cJSON_AddBoolToObject(debug_obj, "servingCellFound", cell_info != NULL);
        if (cell_info) {
            cJSON_AddNumberToObject(debug_obj, "cellRadius", cell_info->cell_radius);
            cJSON_AddNumberToObject(debug_obj, "cellLatitude", cell_info->latitude);
            cJSON_AddNumberToObject(debug_obj, "cellLongitude", cell_info->longitude);
        }
        cJSON_AddStringToObject(debug_obj, "calculationMethod", "cell_center");
        cJSON_AddStringToObject(debug_obj, "cellType", is_ncgi ? "NCGI" : "ECGI");
        cJSON_AddItemToObject(root, "debug", debug_obj);
    }
    
    /* Use unformatted JSON for consistency with Open5GS and better jq compatibility */
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    
    if (!json_str) {
        ogs_error("[%s] cJSON_PrintUnformatted failed", location_request->supi);
        ogs_sbi_message_free(&message);
        return OGS_ERROR;
    }
    
    ogs_info("[%s] LocationData JSON: %s", location_request->supi, json_str);
    
    /* Build HTTP response */
    response = ogs_sbi_response_new();
    ogs_assert(response);
    response->status = OGS_SBI_HTTP_STATUS_OK;
    response->http.content = json_str;
    response->http.content_length = strlen(json_str);
    ogs_sbi_header_set(response->http.headers, "Content-Type", "application/json");

    /* Free message (which frees LocationData) - response has copied the data */
    ogs_sbi_message_free(&message);

    /* Find stream for async response */
    stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
    if (!stream) {
        ogs_error("[%s] Stream ID=%d not found for async response",
                location_request->supi, location_request->stream_id);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    /* Send async response */
    rv = ogs_sbi_server_send_response(stream, response);
    if (!rv) {
        ogs_error("[%s] ogs_sbi_server_send_response() failed",
                location_request->supi);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    ogs_info("[%s] Location response sent successfully (CELLID)",
            location_request->supi);

    /* Clean up location request */
    lmf_location_request_remove(location_request);

    return OGS_OK;
}

