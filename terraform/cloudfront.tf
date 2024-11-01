# This cert is already created in my account
data "aws_acm_certificate" "supergoon_cert" {
  domain      = "*.supergoon.com"
  types       = ["AMAZON_ISSUED"]
  most_recent = true
}

locals {
  s3_origin = "s3-origin"
}

resource "aws_cloudfront_origin_access_control" "access_control" {
  name                              = "EtfAccessControl"
  description                       = "ETF access control"
  origin_access_control_origin_type = "s3"
  signing_behavior                  = "always"
  signing_protocol                  = "sigv4"
}

# Create a CloudFront distribution pointing to the S3 bucket
resource "aws_cloudfront_distribution" "supergoon_world_distribution" {
  origin {
    domain_name = aws_s3_bucket.supergoon_world_bucket.bucket_regional_domain_name
    origin_access_control_id = aws_cloudfront_origin_access_control.access_control.id
    origin_id   = local.s3_origin
  }
  enabled             = true
  default_root_object = "EscapeTheFate.html" # The main HTML file for your website
  aliases = ["escapethefate.supergoon.com"]

  restrictions {
    geo_restriction {
      restriction_type = "none"
    }
  }


  viewer_certificate {
    acm_certificate_arn = data.aws_acm_certificate.supergoon_cert.arn
    ssl_support_method  = "sni-only"
  }


  default_cache_behavior {
    allowed_methods  = ["DELETE", "GET", "HEAD", "OPTIONS", "PATCH", "POST", "PUT"]
    cached_methods   = ["HEAD", "GET"]
    target_origin_id = local.s3_origin

    forwarded_values {
      query_string = false

      cookies {
        forward = "none"
      }
    }

    # viewer_protocol_policy = "allow-all"
    min_ttl                = 0
    default_ttl            = 60
    max_ttl                = 86400
    viewer_protocol_policy = "redirect-to-https"
  }
  price_class = "PriceClass_100"
}

resource "aws_s3_bucket_policy" "cloudfront_access_policy" {
  bucket = aws_s3_bucket.supergoon_world_bucket.id
  policy = jsonencode({
    Version = "2012-10-17",
    Statement = [{
      Action = "s3:GetObject",
      Effect = "Allow",
      Resource = "${aws_s3_bucket.supergoon_world_bucket.arn}/*",
      Principal = {
        Service = "cloudfront.amazonaws.com"
      }
      Condition = {
        StringEquals = {
            "AWS:SourceArn" = aws_cloudfront_distribution.supergoon_world_distribution.arn
        }
      }
    }]
  })
}
