/*
 * (C) Copyright 2013 Kurento (http://kurento.org/)
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 */
#ifndef __KMS_BASE_SDP_ENDPOINT_H__
#define __KMS_BASE_SDP_ENDPOINT_H__

#include <gst/gst.h>
#include <gst/sdp/gstsdpmessage.h>
#include "kmselement.h"
#include "sdpagent/kmssdpagent.h"
#include "sdpagent/kmssdpcontext.h"
#include "sdpagent/kmssdppayloadmanager.h"
#include "kmsrefstruct.h"

G_BEGIN_DECLS

/* #defines don't like whitespacey bits */
#define KMS_TYPE_BASE_SDP_ENDPOINT \
  (kms_base_sdp_endpoint_get_type())
#define KMS_BASE_SDP_ENDPOINT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),KMS_TYPE_BASE_SDP_ENDPOINT,KmsBaseSdpEndpoint))
#define KMS_BASE_SDP_ENDPOINT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),KMS_TYPE_BASE_SDP_ENDPOINT,KmsBaseSdpEndpointClass))
#define KMS_IS_BASE_SDP_ENDPOINT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),KMS_TYPE_BASE_SDP_ENDPOINT))
#define KMS_IS_BASE_SDP_ENDPOINT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),KMS_TYPE_BASE_SDP_ENDPOINT))
#define KMS_BASE_SDP_ENDPOINT_CAST(obj) ((KmsBaseSdpEndpoint*)(obj))

typedef struct _KmsBaseSdpEndpointPrivate KmsBaseSdpEndpointPrivate;
typedef struct _KmsBaseSdpEndpoint KmsBaseSdpEndpoint;
typedef struct _KmsBaseSdpEndpointClass KmsBaseSdpEndpointClass;

#define KMS_BASE_SDP_ENDPOINT_LOCK(elem) \
  (g_rec_mutex_lock (&KMS_BASE_SDP_ENDPOINT_CAST ((elem))->media_mutex))
#define KMS_BASE_SDP_ENDPOINT_UNLOCK(elem) \
  (g_rec_mutex_unlock (&KMS_BASE_SDP_ENDPOINT_CAST ((elem))->media_mutex))

typedef struct _KmsSdpSession
{
  KmsRefStruct ref;

  guint id;
  gchar *id_str;

  KmsBaseSdpEndpoint *sdp_ep;
  KmsSdpAgent *agent;
  KmsSdpPayloadManager *ptmanager;

  SdpMessageContext *local_ctx;
  SdpMessageContext *remote_ctx;
  SdpMessageContext *negotiated_ctx;
} KmsSdpSession;

struct _KmsBaseSdpEndpoint
{
  KmsElement parent;

  KmsBaseSdpEndpointPrivate *priv;
};

struct _KmsBaseSdpEndpointClass
{
  KmsElementClass parent_class;

  /* private */
  /* actions */
  const gchar* (*create_session) (KmsBaseSdpEndpoint * self);
  gboolean (*release_session) (KmsBaseSdpEndpoint * self, const gchar *sess_id);

  GstSDPMessage *(*generate_offer) (KmsBaseSdpEndpoint * self, const gchar *sess_id);
  GstSDPMessage *(*process_offer) (KmsBaseSdpEndpoint * self, const gchar *sess_id, GstSDPMessage * offer);
  void (*process_answer) (KmsBaseSdpEndpoint * self, const gchar *sess_id, GstSDPMessage * answer);

  GstSDPMessage *(*get_local_sdp) (KmsBaseSdpEndpoint * self, const gchar *sess_id);
  GstSDPMessage *(*get_remote_sdp) (KmsBaseSdpEndpoint * self, const gchar *sess_id);

  /* virtual methods */
  void (*start_transport_send) (KmsBaseSdpEndpoint * self, KmsSdpSession *sess, gboolean offerer);
  void (*connect_input_elements) (KmsBaseSdpEndpoint * self, KmsSdpSession *sess);

  gboolean (*configure_media) (KmsBaseSdpEndpoint * self, KmsSdpSession *sess, SdpMediaConfig * mconf);

  /* Virtual handler factory methods */
  void (*create_media_handler) (KmsBaseSdpEndpoint * self, const gchar *media, KmsSdpMediaHandler **handler);
};

GType kms_base_sdp_endpoint_get_type (void);

KmsSdpSession * kms_base_sdp_endpoint_get_session (KmsBaseSdpEndpoint * self, const gchar *sess_id);
KmsSdpSession * kms_base_sdp_endpoint_get_first_negotiated_session (KmsBaseSdpEndpoint * self);

G_END_DECLS
#endif /* __KMS_BASE_SDP_ENDPOINT_H__ */
